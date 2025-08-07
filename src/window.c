#include <core.h>
#include "_get_fn_.h"

static SDL_Window* sdl_win;
static SDL_Renderer* sdl_renderer;
static SDL_Texture* sdl_texture;
static Texture buffer;

SDL_Window *_sdl_getwin(void) { return sdl_win; }
SDL_Renderer *_sdl_getrenderer(void) { return sdl_renderer; }
SDL_Texture *_sdl_gettexture(void) { return sdl_texture; }
Texture _get_buffer(void) { return buffer; }

WindowOptions windowoptions_default(void) {
  return (WindowOptions){
    .title = "nal.c window",
    .size = (vec2u){800, 600},
    .buffer_size = (vec2u){800, 600},
    .full_screen = false,
    .hidden = false,
    .borderless = false,
    .resizable = false,
    .minimized = false,
    .maximized = false,
    .input_grabbed = false,
  };
}
static u32 windowoptions_load(WindowOptions opt) {
  u32 flags = SDL_WINDOW_SHOWN;

  if (opt.full_screen) flags |= SDL_WINDOW_FULLSCREEN;
  if (opt.hidden) flags |= SDL_WINDOW_HIDDEN;
  if (opt.borderless) flags |= SDL_WINDOW_BORDERLESS;
  if (opt.resizable) flags |= SDL_WINDOW_RESIZABLE;
  if (opt.minimized) flags |= SDL_WINDOW_MINIMIZED;
  if (opt.maximized) flags |= SDL_WINDOW_MAXIMIZED;
  if (opt.input_grabbed) flags |= SDL_WINDOW_INPUT_GRABBED;

  return flags;
}

void window_create(char *title, vec2u size, vec2u buffer_size) {
  WindowOptions opt = windowoptions_default();
  opt.title = title;
  opt.size = size;
  opt.buffer_size = buffer_size;
  window_createx(opt);
}
void window_createx(WindowOptions options) {
  SDL_Init(SDL_INIT_VIDEO);
  u32 flags = windowoptions_load(options);
  sdl_win = SDL_CreateWindow(options.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      options.size.x, options.size.y, flags);
  sdl_renderer = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
  sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, options.buffer_size.x, options.buffer_size.y);

  buffer = (Texture){
    .size = (vec2u){options.buffer_size.x, options.buffer_size.y},
    .data = malloc(sizeof(Color)*options.buffer_size.x*options.buffer_size.y),
  };
  if (!buffer.data)
    FATAL("failed to allocate mem for buffer");

  for (u32 i=0;i<buffer.size.x*buffer.size.y;i++)
    buffer.data[i] = 0xffffffff;

  INFO("created window and buffer successfully");
  INFO("buffer: %ix%i", buffer.size.x, buffer.size.y);
}

void window_destroy(void) {
  free(buffer.data);
  buffer.data = NULL;
  SDL_DestroyTexture(sdl_texture);
  sdl_texture = NULL;
  SDL_DestroyRenderer(sdl_renderer);
  sdl_renderer = NULL;
  SDL_DestroyWindow(sdl_win);
  sdl_win = NULL;

  SDL_Quit();
} 

