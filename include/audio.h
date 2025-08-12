#pragma once

typedef void* Sound;

void sound_load(Sound *s, const char *path);
void sound_play(Sound *s);
