#pragma once

#include <utils.h>

typedef u32 Color;

void 

void render_pixel(vec2 pos, Color color);

typedef struct {
  Color *data;
  vec2u size;
} Texture;


