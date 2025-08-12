#include <input.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "__global__.h"

static bool _key_current[KEY_COUNT] = {0};
static bool _key_previous[KEY_COUNT] = {0};

static bool  _mouse_current[MOUSE_COUNT] = {0};
static bool  _mouse_previous[MOUSE_COUNT] = {0};
static i32   _mouse_scroll = 0;
static vec2u _mouse_pos = {0};

void _input_update(void) {
  memcpy(_key_previous, _key_current, sizeof(_key_previous));
  const u8 *keystate = SDL_GetKeyboardState(NULL);
  for (u32 i=0;i<KEY_COUNT;i++) {
    _key_current[i] = keystate[i];
  }

  memcpy(_mouse_previous, _mouse_current, sizeof(_mouse_previous));
  for (u8 i=0;i<MOUSE_COUNT;i++)
    _mouse_current[i] = 0;
  u32 mousestate = SDL_GetMouseState(NULL, NULL);
  if (mousestate & SDL_BUTTON(SDL_BUTTON_LEFT))
    _mouse_current[MOUSE_LEFT] = 1;
  if (mousestate & SDL_BUTTON(SDL_BUTTON_RIGHT))
    _mouse_current[MOUSE_RIGHT] = 1;
  if (mousestate & SDL_BUTTON(SDL_BUTTON_MIDDLE))
    _mouse_current[MOUSE_MIDDLE] = 1;
  if (mousestate & SDL_BUTTON(SDL_BUTTON_X1))
    _mouse_current[MOUSE_BUTTON4] = 1;
  if (mousestate & SDL_BUTTON(SDL_BUTTON_X2))
    _mouse_current[MOUSE_BUTTON5] = 1;
}

void _input_updatelate(void) {
  SDL_Event e;
  bool *running = _get_running();
  _mouse_scroll = 0;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      *running = 0;
      INFO("running state set to 0");
    }
    if (e.type == SDL_MOUSEMOTION) {
      _mouse_pos.x = e.motion.x;
      _mouse_pos.y = e.motion.y;
    }
    if (e.type == SDL_MOUSEWHEEL) {
      _mouse_scroll = e.wheel.y;
    }
  }

}

bool input_pressed(Key k) {
  return _key_current[k] & (!_key_previous[k]);
} 

bool input_released(Key k) {
  return _key_previous[k] & (!_key_current[k]);
}

bool input_down(Key k) {
  return _key_current[k];
}

vec2u mouse_pos(void) {
  return _mouse_pos;
}
vec2u mouse_posw(void) {
  vec2u res = {0};
  
  vec2u window = _get_windowsize();

  res = vec2u_div(vec2u_mul(mouse_pos(), _get_buffer()->size), window);

  return res;
}
bool mouse_down(MouseButton btn) {
  return _mouse_current[btn];
}
bool mouse_pressed(MouseButton btn) {
  return (!_mouse_previous[btn]) & _mouse_current[btn];
}
bool mouse_released(MouseButton btn) {
  return (!_mouse_current[btn]) & _mouse_previous[btn];
}
i32 mouse_scroll(void) {
  return _mouse_scroll;
}
