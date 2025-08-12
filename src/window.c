#include <core.h>
#include "__global__.h"

static SDL_Window* sdl_win;
static SDL_Renderer* sdl_renderer;
static SDL_Texture* sdl_texture;
static Texture buffer;
static vec2u window_size;
static f32 time_delta;
static f32 time_target = 0;
static f32 time_last;
static f32 time_start;
static bool running = 0;

SDL_Window *_sdl_getwin(void) { return sdl_win; }
SDL_Renderer *_sdl_getrenderer(void) { return sdl_renderer; }
SDL_Texture *_sdl_gettexture(void) { return sdl_texture; }
Texture *_get_buffer(void) { return &buffer; }
bool *_get_running(void) { return &running; }
vec2u _get_windowsize(void) { return window_size; }

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

void time_set_target(f32 target) {
  time_target = 1000.0 / target;
}

f32 time_get_delta(void) {
  return time_delta;
}

void window_create(char *title, vec2u size, vec2u buffer_size) {
  WindowOptions opt = windowoptions_default();
  opt.title = title;
  opt.size = size;
  opt.buffer_size = buffer_size;
  window_createx(opt);
}
void window_createx(WindowOptions options) {
  running = true;
  window_size = options.size;

  _audio_init();

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

  if (time_target == 0) {
    time_target = 1000.0 / 60.0;
    INFO("no target fps specified, auto-setting to 60 fps");
  }
  time_delta = 0.0;
  time_last = SDL_GetTicks();
}

void window_update(void) {
  time_start = SDL_GetTicks();
  _input_update();
}
void window_updatelate(void) {
  SDL_GetWindowSize(sdl_win, &window_size.x, &window_size.y);

  f32 sx = (f32)window_size.x / (f32)buffer.size.x;
  f32 sy = (f32)window_size.y / (f32)buffer.size.y;
  f32 scale = (sx < sy) ? sx : sy;

  SDL_Rect dst = {
    (window_size.x-buffer.size.x*scale)/2,
    (window_size.y-buffer.size.y*scale)/2,
    buffer.size.x*scale, buffer.size.y*scale
  };

  SDL_UpdateTexture(sdl_texture, NULL, buffer.data, buffer.size.x*sizeof(Color));
  SDL_RenderClear(sdl_renderer);
  SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, &dst);
  SDL_RenderPresent(sdl_renderer);

  u32 frame_time = SDL_GetTicks() - time_start;
  if (frame_time < time_target)
    SDL_Delay(time_target - frame_time);

  u32 current_time = SDL_GetTicks();
  time_delta = (current_time - time_last) / 1000.0f;
  time_last = current_time;

  _input_updatelate();
}

bool window_shouldclose(void) {
  return !running;
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


