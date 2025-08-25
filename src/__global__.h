#pragma once

#include <windows.h>
#include <render.h>

Texture *_get_buffer(void);
vec2u _get_windowsize(void);
bool *_get_running(void);

void _input_update(void);
void _input_setkey(WPARAM wparam, bool down);
void _input_setmouse(UINT msg, WPARAM wparam, LPARAM lparam);
void _audio_init(void);
