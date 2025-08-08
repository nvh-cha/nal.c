#pragma once

#include <render.h>
#include <SDL2/SDL.h>

SDL_Window *_sdl_getwin(void);
SDL_Renderer *_sdl_getrenderer(void);
SDL_Texture *_sdl_gettexture(void);

Texture *_get_buffer(void);

void _input_update(void);
