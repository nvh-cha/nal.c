#include <input.h>
#include <string.h>
#include <SDL2/SDL.h>

static bool _key_current[KEY_COUNT] = {0};
static bool _key_previous[KEY_COUNT] = {0};

void _input_update(void) {
  memcpy(_key_previous, _key_current, sizeof(_key_previous));
  const u8 *keystate = SDL_GetKeyboardState(NULL);
  for (u32 i=0;i<KEY_COUNT;i++) {
    _key_current[i] = keystate[i];
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
