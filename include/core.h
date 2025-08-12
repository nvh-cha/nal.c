#pragma once

#include <utils.h>

typedef struct {
  char *title;
  vec2u size;
  vec2u buffer_size;
  bool full_screen;
  bool hidden;
  bool borderless;
  bool resizable;
  bool minimized;
  bool maximized;
  bool input_grabbed;
} WindowOptions;
WindowOptions windowoptions_default(void);

void window_create(char *title, vec2u size, vec2u buffer_size);
void window_createx(WindowOptions options);
void window_update(void);
void window_updatelate(void);
bool window_shouldclose(void);
void window_destroy(void);

void time_settarget(f32 target);
f32 time_getdelta(void);
