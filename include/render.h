#pragma once

#include <utils.h>
#include <core.h>

typedef u32 Color;

Color color_pack(u8 a, u8 r, u8 g, u8 b);
void color_unpack(Color c, u8 *a, u8 *r, u8 *g, u8 *b);

#define COLOR_RED 0xffff0000
#define COLOR_GREEN 0xff00ff00
#define COLOR_BLUE 0xff0000ff
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0xff000000
#define COLOR_MAGENTA 0xffff00ff
#define COLOR_YELLOW 0xffffff00
#define COLOR_CYAN 0xff00ffff

typedef struct {
  Color *data;
  vec2u size;
} Texture;

Texture texture_load(const char *path);
Texture texture_copy(Texture t);
void texture_replacecolor(Texture *t, Color from, Color to);
void texture_flipx(Texture *t);
void texture_flipy(Texture *t);
void texture_cut(Texture *t, vec2u pos, vec2u size); // consumes the initial texture
void texture_scale(Texture *t, f32 a);
void texture_scalex(Texture *t, vec2u size);
void texture_free(Texture *t);

typedef struct {
  Texture *images;
  u32 len;
} Spritesheet;

Spritesheet spritesheet_load(const char *path);
Spritesheet spritesheet_loadx(const char *path, vec2u sprite_size);
Texture spritesheet_get(Spritesheet s, u32 i);
void spritesheet_free(Spritesheet s);

typedef struct {
  Spritesheet images;
  u32 i;
  Timer timer;
  bool repeat;
} Animation;

Animation animation_create(char *path, f32 delay, bool repeat);
void animation_free(Animation *ani);

void render_changetarget(Texture *render_target);
void render_clear(Color color);

void render_pixel(vec2 pos, Color color);
void render_rectangle(vec2 pos, vec2 size, Color color);
void render_line(vec2 start, vec2 end, Color color);
void render_texture(Texture image, vec2 pos);
void render_animation(Animation *ani, vec2 pos);
