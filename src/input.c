#include <input.h>
#include <windows.h>
#include <string.h>
#include "__global__.h"

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

static Key vk_to_key(WPARAM vk) {
    switch (vk) {
        // Letters
        case 'A': return KEY_A;
        case 'B': return KEY_B;
        case 'C': return KEY_C;
        case 'D': return KEY_D;
        case 'E': return KEY_E;
        case 'F': return KEY_F;
        case 'G': return KEY_G;
        case 'H': return KEY_H;
        case 'I': return KEY_I;
        case 'J': return KEY_J;
        case 'K': return KEY_K;
        case 'L': return KEY_L;
        case 'M': return KEY_M;
        case 'N': return KEY_N;
        case 'O': return KEY_O;
        case 'P': return KEY_P;
        case 'Q': return KEY_Q;
        case 'R': return KEY_R;
        case 'S': return KEY_S;
        case 'T': return KEY_T;
        case 'U': return KEY_U;
        case 'V': return KEY_V;
        case 'W': return KEY_W;
        case 'X': return KEY_X;
        case 'Y': return KEY_Y;
        case 'Z': return KEY_Z;

        // Numbers (top row)
        case '1': return KEY_1;
        case '2': return KEY_2;
        case '3': return KEY_3;
        case '4': return KEY_4;
        case '5': return KEY_5;
        case '6': return KEY_6;
        case '7': return KEY_7;
        case '8': return KEY_8;
        case '9': return KEY_9;
        case '0': return KEY_0;

        // Control keys
        case VK_RETURN: return KEY_RETURN;
        case VK_ESCAPE: return KEY_ESCAPE;
        case VK_BACK:   return KEY_BACKSPACE;
        case VK_TAB:    return KEY_TAB;
        case VK_SPACE:  return KEY_SPACE;

        // Symbols
        case VK_OEM_MINUS:     return KEY_MINUS;
        case VK_OEM_PLUS:      return KEY_EQUALS;
        case VK_OEM_4:         return KEY_LEFTBRACKET;
        case VK_OEM_6:         return KEY_RIGHTBRACKET;
        case VK_OEM_5:         return KEY_BACKSLASH;
        case VK_OEM_1:         return KEY_SEMICOLON;
        case VK_OEM_7:         return KEY_APOSTROPHE;
        case VK_OEM_3:         return KEY_GRAVE;
        case VK_OEM_COMMA:     return KEY_COMMA;
        case VK_OEM_PERIOD:    return KEY_PERIOD;
        case VK_OEM_2:         return KEY_SLASH;

        // Function keys
        case VK_F1:  return KEY_F1;
        case VK_F2:  return KEY_F2;
        case VK_F3:  return KEY_F3;
        case VK_F4:  return KEY_F4;
        case VK_F5:  return KEY_F5;
        case VK_F6:  return KEY_F6;
        case VK_F7:  return KEY_F7;
        case VK_F8:  return KEY_F8;
        case VK_F9:  return KEY_F9;
        case VK_F10: return KEY_F10;
        case VK_F11: return KEY_F11;
        case VK_F12: return KEY_F12;

        // Navigation
        case VK_LEFT:   return KEY_LEFT;
        case VK_RIGHT:  return KEY_RIGHT;
        case VK_UP:     return KEY_UP;
        case VK_DOWN:   return KEY_DOWN;
        case VK_HOME:   return KEY_HOME;
        case VK_END:    return KEY_END;
        case VK_PRIOR:  return KEY_PAGEUP;
        case VK_NEXT:   return KEY_PAGEDOWN;
        case VK_INSERT: return KEY_INSERT;
        case VK_DELETE: return KEY_DELETE;

        // Modifiers
        case VK_LSHIFT: return KEY_LSHIFT;
        case VK_RSHIFT: return KEY_RSHIFT;
        case VK_LCONTROL: return KEY_LCTRL;
        case VK_RCONTROL: return KEY_RCTRL;
        case VK_LMENU: return KEY_LALT;
        case VK_RMENU: return KEY_RALT;
        case VK_LWIN:  return KEY_LGUI;
        case VK_RWIN:  return KEY_RGUI;
        case VK_CAPITAL: return KEY_CAPSLOCK;

        // Numpad
        case VK_NUMLOCK:   return KEY_NUMLOCKCLEAR;
        case VK_DIVIDE:    return KEY_KP_DIVIDE;
        case VK_MULTIPLY:  return KEY_KP_MULTIPLY;
        case VK_SUBTRACT:  return KEY_KP_MINUS;
        case VK_ADD:       return KEY_KP_PLUS;
        case VK_NUMPAD0:   return KEY_KP_0;
        case VK_NUMPAD1:   return KEY_KP_1;
        case VK_NUMPAD2:   return KEY_KP_2;
        case VK_NUMPAD3:   return KEY_KP_3;
        case VK_NUMPAD4:   return KEY_KP_4;
        case VK_NUMPAD5:   return KEY_KP_5;
        case VK_NUMPAD6:   return KEY_KP_6;
        case VK_NUMPAD7:   return KEY_KP_7;
        case VK_NUMPAD8:   return KEY_KP_8;
        case VK_NUMPAD9:   return KEY_KP_9;
        case VK_DECIMAL:   return KEY_KP_PERIOD;
    }

    return KEY_UNKNOWN;
}

static bool _key_current[KEY_COUNT] = {0};
static bool _key_previous[KEY_COUNT] = {0};

static bool  _mouse_current[MOUSE_COUNT] = {0};
static bool  _mouse_previous[MOUSE_COUNT] = {0};
static i32   _mouse_scroll = 0;
static vec2u _mouse_pos = {0};

void _input_update(void) {
  memcpy(_key_previous, _key_current, sizeof(_key_previous));
  memcpy(_mouse_previous, _mouse_current, sizeof(_mouse_previous));

  _mouse_scroll = 0;
}

void _input_setkey(WPARAM wParam, bool down) {
    Key k = vk_to_key(wParam);
    if (k != KEY_UNKNOWN) {
        _key_current[k] = down;
    }
}

void _input_setmouse(UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_LBUTTONDOWN: _mouse_current[MOUSE_LEFT]   = true; break;
    case WM_LBUTTONUP:   _mouse_current[MOUSE_LEFT]   = false; break;
    case WM_RBUTTONDOWN: _mouse_current[MOUSE_RIGHT]  = true; break;
    case WM_RBUTTONUP:   _mouse_current[MOUSE_RIGHT]  = false; break;
    case WM_MBUTTONDOWN: _mouse_current[MOUSE_MIDDLE] = true; break;
    case WM_MBUTTONUP:   _mouse_current[MOUSE_MIDDLE] = false; break;
    case WM_XBUTTONDOWN:
                         if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) _mouse_current[MOUSE_BUTTON4] = true;
                         if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) _mouse_current[MOUSE_BUTTON5] = true;
                         break;
    case WM_XBUTTONUP:
                         if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) _mouse_current[MOUSE_BUTTON4] = false;
                         if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) _mouse_current[MOUSE_BUTTON5] = false;
                         break;
    case WM_MOUSEMOVE:
                         _mouse_pos.x = GET_X_LPARAM(lParam);
                         _mouse_pos.y = GET_Y_LPARAM(lParam);
                         break;
    case WM_MOUSEWHEEL:
                         _mouse_scroll = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
                         break;
  }
}

bool input_pressed(Key k) {
  return _key_current[k] && !_key_previous[k];
}

bool input_released(Key k) {
  return _key_previous[k] && !_key_current[k];
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
  return !_mouse_previous[btn] && _mouse_current[btn];
}

bool mouse_released(MouseButton btn) {
  return !_mouse_current[btn] && _mouse_previous[btn];
}

i32 mouse_scroll(void) {
  return _mouse_scroll;
}
