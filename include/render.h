#pragma once

#include <utils.h>
#include <core.h>

typedef u32 Color;

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

void render_changetarget(Texture *render_target);
void render_clear(Color color);

void render_pixel(vec2 pos, Color color);
void render_rectangle(vec2 pos, vec2 size, Color color);
void render_line(vec2 start, vec2 end, Color color);
void render_texture(Texture image, vec2 pos);
