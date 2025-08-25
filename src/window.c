#include <windows.h>
#include <core.h>
#include "__global__.h"

#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)

static HWND win_handle;
static HDC win_dc;
static HBITMAP backbuffer_bitmap;
static BITMAPINFO backbuffer_info;
static Texture buffer;
static vec2u window_size;
static f32 time_delta;
static f32 time_target = 0;
static f32 time_last;
static f32 time_start;
static bool running = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            *_get_running() = false;
            PostQuitMessage(0);
            return 0;

        // keyboard
        case WM_KEYDOWN: _input_setkey(wParam, true); return 0;
        case WM_KEYUP:   _input_setkey(wParam, false); return 0;

        // mouse
        case WM_LBUTTONDOWN: case WM_LBUTTONUP:
        case WM_RBUTTONDOWN: case WM_RBUTTONUP:
        case WM_MBUTTONDOWN: case WM_MBUTTONUP:
        case WM_XBUTTONDOWN: case WM_XBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
            _input_setmouse(uMsg, wParam, lParam);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Texture *_get_buffer(void) { return &buffer; }
bool *_get_running(void) { return &running; }
vec2u _get_windowsize(void) { return window_size; }

WindowOptions windowoptions_default(void) {
  return (WindowOptions){
    .title = "nal.c window",
    .size = (vec2u){800, 600},
    .buffer_size = (vec2u){800, 600},
    .resizable = false,
    .hidden = false,
  };
}

void time_settarget(f32 target) {
  time_target = 1000.0f / target;
}

f32 time_getdelta(void) {
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

  //_audio_init();

  // Register window class
  WNDCLASS wc = {0};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "nalc_window_class";
  RegisterClass(&wc);

  DWORD style = WS_OVERLAPPEDWINDOW;
  if (options.borderless) style = WS_POPUP;
  if (!options.resizable) {
    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
  }

  RECT rect = {0, 0, options.size.x, options.size.y};
  AdjustWindowRect(&rect, style, FALSE);

  win_handle = CreateWindowEx(
      0, wc.lpszClassName, options.title,
      style,
      CW_USEDEFAULT, CW_USEDEFAULT,
      rect.right - rect.left, rect.bottom - rect.top,
      NULL, NULL, wc.hInstance, NULL);

  if (!options.hidden) {
    ShowWindow(win_handle, SW_SHOW);
  }

  win_dc = GetDC(win_handle);

  // Create back buffer
  buffer = (Texture){
    .size = (vec2u){options.buffer_size.x, options.buffer_size.y},
    .data = malloc(sizeof(Color) * options.buffer_size.x * options.buffer_size.y),
  };
  if (!buffer.data)
    FATAL("failed to allocate mem for buffer");

  for (u32 i = 0; i < buffer.size.x * buffer.size.y; i++)
    buffer.data[i] = 0xffffffff;

  ZeroMemory(&backbuffer_info, sizeof(backbuffer_info));
  backbuffer_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  backbuffer_info.bmiHeader.biWidth = buffer.size.x;
  backbuffer_info.bmiHeader.biHeight = -((LONG)buffer.size.y); // top-down
  backbuffer_info.bmiHeader.biPlanes = 1;
  backbuffer_info.bmiHeader.biBitCount = 32;
  backbuffer_info.bmiHeader.biCompression = BI_RGB;

  if (time_target == 0) {
    time_target = 1000.0f / 60.0f;
    INFO("no target fps specified, auto-setting to 60 fps");
  }

  time_delta = 0.0f;
  time_last = (f32)GetTickCount();
  INFO("created window and buffer successfully");
}

void window_update(void) {
  time_start = (f32)GetTickCount();

  _input_update();

  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      running = false;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void window_updatelate(void) {
  RECT client;
  GetClientRect(win_handle, &client);
  window_size.x = client.right - client.left;
  window_size.y = client.bottom - client.top;

  f32 sx = (f32)window_size.x / (f32)buffer.size.x;
  f32 sy = (f32)window_size.y / (f32)buffer.size.y;
  f32 scale = (sx < sy) ? sx : sy;

  int dst_w = (int)(buffer.size.x * scale);
  int dst_h = (int)(buffer.size.y * scale);
  int dst_x = (window_size.x - dst_w) / 2;
  int dst_y = (window_size.y - dst_h) / 2;

  StretchDIBits(
      win_dc,
      dst_x, dst_y, dst_w, dst_h,
      0, 0, buffer.size.x, buffer.size.y,
      buffer.data, &backbuffer_info, DIB_RGB_COLORS, SRCCOPY);

  u32 frame_time = (u32)(GetTickCount() - time_start);
  if (frame_time < time_target)
    Sleep((DWORD)(time_target - frame_time));

  u32 current_time = GetTickCount();
  time_delta = (current_time - time_last) / 1000.0f;
  time_last = current_time;

  //_input_updatelate();
}

bool window_shouldclose(void) {
  return !running;
}

void window_destroy(void) {
  free(buffer.data);
  buffer.data = NULL;
  ReleaseDC(win_handle, win_dc);
  DestroyWindow(win_handle);
}
