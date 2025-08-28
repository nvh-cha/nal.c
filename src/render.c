#include <math.h>
#include <render.h>
#include "__global__.h"

static Texture *target;
static BlendMode blend_mode = BLEND_NORMAL;

Color color_pack(u8 a, u8 r, u8 g, u8 b) {
  return ((u32)a << 24) | ((u32)r << 16) | ((u32)g << 8) | ((u32)b);
}

void color_unpack(Color c, u8 *a, u8 *r, u8 *g, u8 *b) {
  *a = (c >> 24) & 0xff;
  *r = (c >> 16) & 0xff;
  *g = (c >> 8)  & 0xff;
  *b = c         & 0xff;
}

static Color blend(Color src, vec2u dst) {
  u8 src_a = 0;
  u8 src_r = 0;
  u8 src_g = 0;
  u8 src_b = 0;
  color_unpack(src, &src_a, &src_r, &src_g, &src_b);
  u8 dst_a = 0;
  u8 dst_r = 0;
  u8 dst_g = 0;
  u8 dst_b = 0;
  color_unpack(target->data[(u32)(dst.x+dst.y*target->size.x)], &dst_a, &dst_r, &dst_g, &dst_b);
  u8 out_a = 0;
  u8 out_r = 0;
  u8 out_g = 0;
  u8 out_b = 0;
  switch (blend_mode) {
    default:
      out_a = src_a;
      out_r = src_r;
      out_g = src_g;
      out_b = src_b;
    break;

    case BLEND_NORMAL:
      out_a = (src_a * src_a + dst_a * (255 - src_a)) / 255;
      out_r = (src_r * src_a + dst_r * (255 - src_a)) / 255;
      out_g = (src_g * src_a + dst_g * (255 - src_a)) / 255;
      out_b = (src_b * src_a + dst_b * (255 - src_a)) / 255;
    break;

    case BLEND_ADDITIVE:
      out_a = src_a * src_a / 255 + dst_a;
      out_r = src_r * src_a / 255 + dst_r;
      out_g = src_g * src_a / 255 + dst_g;
      out_b = src_b * src_a / 255 + dst_b;
    break;

    case BLEND_SUBTRACTIVE:
      out_a = (dst_a - src_a * src_a / 255) < 0 ? 0 : dst_a - src_a * src_a / 255;
      out_r = (dst_r - src_r * src_a / 255) < 0 ? 0 : dst_r - src_r * src_a / 255;
      out_g = (dst_g - src_g * src_a / 255) < 0 ? 0 : dst_g - src_g * src_a / 255;
      out_b = (dst_b - src_b * src_a / 255) < 0 ? 0 : dst_b - src_b * src_a / 255;
    break;

    case BLEND_MULTIPLY:
      out_a = src_a * dst_a / 255;
      out_r = src_r * dst_r / 255;
      out_g = src_g * dst_g / 255;
      out_b = src_b * dst_b / 255;
    break;

    case BLEND_SCREEN:
      out_a = 255 - ((255 - src_a) * (255 - dst_a)) / 255;
      out_r = 255 - ((255 - src_r) * (255 - dst_r)) / 255;
      out_g = 255 - ((255 - src_g) * (255 - dst_g)) / 255;
      out_b = 255 - ((255 - src_b) * (255 - dst_b)) / 255;
    break;

    case BLEND_DARKEN:
      out_a = fmin(src_a, dst_a);
      out_r = fmin(src_r, dst_r);
      out_g = fmin(src_g, dst_g);
      out_b = fmin(src_b, dst_b);
    break;

    case BLEND_LIGHTEN:
      out_a = fmax(src_a, dst_a);
      out_r = fmax(src_r, dst_r);
      out_g = fmax(src_g, dst_g);
      out_b = fmax(src_b, dst_b);
    break;
  }
  return color_pack(out_a, out_r, out_g, out_b);
}

void render_clear(Color color) {
  for (u32 i=0;i<target->size.x*target->size.y;i++) {
    target->data[i] = color;
  }
}

void render_changeblending(BlendMode mode) {
  blend_mode = mode;
}

void render_changetarget(Texture *render_target) {
  if (render_target == NULL)
    target = _get_buffer();
  else
    target = render_target;
}

void render_pixel(vec2 pos, Color color) {
  if (pos.x >= 0 && pos.x < target->size.x &&
      pos.y >= 0 && pos.y < target->size.y) {
    u32 x = (u32)pos.x;
    u32 y = (u32)pos.y;
    Color output = blend(color, (vec2u){x, y});
    target->data[y * target->size.x + x] = output;
  }
}

void render_rectangle(vec2 pos, vec2 size, Color color) {
  for (u32 i=0;i<size.y;i++) {
    for (u32 j=0;j<size.x;j++) {
      render_pixel((vec2){pos.x+j, pos.y+i}, color);
    }
  }
}

void render_line(vec2 start, vec2 end, Color color) {
  f32 dx = end.x - start.x;
  f32 dy = end.y - start.y;

  u32 steps = (i32)fmax(fabs(dx), fabs(dy));

  f32 x_inc = dx / steps;
  f32 y_inc = dy / steps;

  f32 x = start.x;
  f32 y = start.y;

  for (uint32_t i=0;i<=steps;i++) {
    render_pixel((vec2){(int)(x+0.5f), (int)(y+0.5f)}, color);
    x += x_inc;
    y += y_inc;
  }
}

void render_texture(Texture image, vec2 pos) {
  for (u32 i=0;i<image.size.y;i++) {
    for (u32 j=0;j<image.size.x;j++) {
      render_pixel((vec2){pos.x+(f32)j, pos.y+(f32)i}, image.data[j+i*image.size.x]);
    }
  }
}

void render_animation(Animation *ani, vec2 pos) {
  timer_update(&ani->timer);

  if (ani->timer.end) {
    ani->i++;
    if ((ani->i == ani->images.len) && ani->repeat)
      ani->i = 0;
    else if ((ani->i == ani->images.len) && !ani->repeat)
      ani->i = ani->images.len - 1;
  }

  render_texture(
    spritesheet_get(ani->images, ani->i),
    pos
  );
}
