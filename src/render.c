#include <math.h>
#include <render.h>
#include "__global__.h"

Color color_pack(u8 a, u8 r, u8 g, u8 b) {
  return ((u32)a << 24) | ((u32)r << 16) | ((u32)g << 8) | ((u32)b);
}

void color_unpack(Color c, u8 *a, u8 *r, u8 *g, u8 *b) {
  *a = (c >> 24) & 0xff;
  *r = (c >> 16) & 0xff;
  *g = (c >> 8)  & 0xff;
  *b = c         & 0xff;
}

static Texture *target;

void render_clear(Color color) {
  for (u32 i=0;i<target->size.x*target->size.y;i++) {
    target->data[i] = color;
  }
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
    target->data[(u32)(pos.x+pos.y*target->size.x)] = color;
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
