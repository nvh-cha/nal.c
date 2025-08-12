#include <render.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture texture_load(const char *path) {
  u32 w, h, _channels;
  u8* image_data = stbi_load(path, &w, &h, &_channels, 4);
  if (!image_data)
    FATAL("failed to load image: %s", path);

  Texture res = (Texture){0};
  res.size = (vec2u){w, h};
  res.data = malloc(sizeof(Color) * w * h);
  if (!res.data)
    FATAL("failed to allocate mem for image %s", path);

  for (u32 i=0;i<w*h;i++) {
    u8 r = image_data[i * 4 + 0];
    u8 g = image_data[i * 4 + 1];
    u8 b = image_data[i * 4 + 2];
    u8 a = image_data[i * 4 + 3];
    res.data[i] = (a << 24) | (r << 16) | (g << 8) | b;
  }

  stbi_image_free(image_data);
  
  INFO("loaded texture: %s %ix%i", path, w, h);

  return res;
}
Texture texture_copy(Texture t) {
  Texture res = (Texture) { .size = t.size };
  res.data = malloc(sizeof(Color)*t.size.x*t.size.y);
  if (!res.data)
    FATAL("failed to allocate mem for texture copy");

  for (u32 i=0;i<t.size.x*t.size.y;i++)
    res.data[i] = t.data[i];

  return res;
}
void texture_replacecolor(Texture *t, Color from, Color to) {
  for (u32 i=0;i<t->size.x*t->size.y;i++) {
    if (t->data[i] == from)
      t->data[i] = to;
  }
}
void texture_flipy(Texture *t) {
  Texture tmp = texture_copy(*t);
  for (u32 i=0;i<t->size.x;i++)
    for (u32 j=0;j<t->size.y;j++)
      t->data[j*t->size.x+i] = tmp.data[(t->size.y-1-j) * t->size.x+i];
  free(tmp.data);
}
void texture_flipx(Texture *t) {
  Texture tmp = texture_copy(*t);
  for (u32 i=0;i<t->size.y;i++)
    for (u32 j=0;j<t->size.x;j++)
      t->data[i*t->size.x+j] = tmp.data[i*t->size.x+(t->size.x-j)];
  free(tmp.data);
}
void texture_cut(Texture *t, vec2u pos, vec2u size) {
  Texture tmp = texture_copy(*t);

  free(t->data);
  t->size = size;
  t->data = malloc(sizeof(Color)*size.x*size.y);
  if (!t->data)
    FATAL("failed to allocate image for texture cut");

  for (u32 i=0;i<size.y;i++)
    for (u32 j=0;j<size.x;j++)
      t->data[j+i*size.x] = tmp.data[(pos.y+i)*tmp.size.x+pos.x+j];

  free(tmp.data);
}
void texture_scale(Texture *t, f32 a) {
  vec2u target = (vec2u){t->size.x*a, t->size.y*a};
  texture_scalex(t, target);
}
void texture_scalex(Texture *t, vec2u size) {
  Texture tmp = texture_copy(*t);
  free(t->data);
  t->size = size;
  t->data = malloc(sizeof(Color)*size.x*size.y);
  if (!t->data)
    FATAL("failed to malloc for texture scale");

  vec2f ratio = vec2f_div(vec2u_tof(tmp.size), vec2u_tof(size));

  for (u32 y = 0;y<size.y;y++) {
    i32 src_y = (i32)(y*ratio.y);
    for (u32 x = 0;x<size.x;x++) {
      i32 src_x = (i32)(x*ratio.x);
      t->data[y*size.x+x] = tmp.data[src_y*tmp.size.x+src_x];
    }
  }

  free(tmp.data);
}
void texture_free(Texture *t) {
  free(t->data);
  t->data = NULL;

  INFO("unloaded texture %ix%i", t->size.x, t->size.y);
}

Spritesheet spritesheet_load(const char *path) {
  Texture atlas = texture_load(path);

  vec2u sprite_size = (vec2u){0};
  Color delimiter = atlas.data[0];
  sprite_size.x++;
  while (atlas.data[sprite_size.x] != delimiter)
    sprite_size.x++;
  sprite_size.x--;

  sprite_size.y++;
  while (atlas.data[sprite_size.y*atlas.size.x] != delimiter)
    sprite_size.y++;
  sprite_size.y--;

  texture_free(&atlas);

  return spritesheet_loadx(path, sprite_size);
}
Spritesheet spritesheet_loadx(const char *path, vec2u sprite_size) {
  Spritesheet res;
  Texture atlas = texture_load(path);

  res.len = ((atlas.size.x/sprite_size.x)*(atlas.size.y/sprite_size.y));
  res.images = malloc(sizeof(Texture)*res.len);

  for (u32 y=0;y<atlas.size.y/sprite_size.y;y++) {
    for (u32 x=0;x<atlas.size.x/sprite_size.x;x++) {
      Texture t = texture_copy(atlas);
      texture_cut(&t, (vec2u){x*sprite_size.x+(x+1), y*sprite_size.y+(y+1)}, sprite_size);
      res.images[y*(atlas.size.x/sprite_size.x)+x] = t;
    }
  }
  
  texture_free(&atlas);

  INFO("loaded spritesheet: %s with sprite size: %ix%i", path, sprite_size.x, sprite_size.y);
  return res;
}
Texture spritesheet_get(Spritesheet s, u32 i) {
  if (i >= s.len)
    FATAL("index out of bounds: %i", i);
  return s.images[i];
}
void spritesheet_free(Spritesheet s) {
  for (u32 i=0;i<s.len;i++)
    free(s.images[i].data);

  INFO("unloaded spritesheet");
}
