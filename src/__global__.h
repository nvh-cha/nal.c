#pragma once

#include <render.h>
#include <SDL2/SDL.h>

SDL_Window *_sdl_getwin(void);
SDL_Renderer *_sdl_getrenderer(void);
SDL_Texture *_sdl_gettexture(void);

Texture *_get_buffer(void);
vec2u _get_windowsize(void);
bool *_get_running(void);

void _input_update(void);
void _input_updatelate(void);
