#include <audio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "__global__.h"

void _audio_init(void) {
  SDL_Init(SDL_INIT_AUDIO);

  i32 rate = 44100;
  u16 format = MIX_DEFAULT_FORMAT;
  i32 channels = 2;
  i32 chunksize = 2048;

  if (Mix_OpenAudio(rate, format, channels, chunksize))
    FATAL("failed to init audio: Openaudio: %s", Mix_GetError());
}

void sound_load(Sound *s, const char *path) {
  *s = Mix_LoadWAV(path);
  if (!*s)
    FATAL("failed to load sound: %s", path);

  INFO("loaded sound: %s", path);
}
void sound_play(Sound *s) {
  Mix_PlayChannel(-1, *s, 0);
}

void music_load(Music *m, const char *path) {
  *m = Mix_LoadMUS(path);
  if (!*m)
    FATAL("failed to load music: %s", path);

  INFO("loaded sound: %s", path);
}
void music_play(Music *m) {
  Mix_PlayMusic(*m, -1);
}

void audio_volume(u32 volume) {
  Mix_Volume(-1, volume);
}
