#include <nal.h>

int main(int argc, char *argv[]) {
  WindowOptions opt = windowoptions_default();
  window_createx(opt);

  Spritesheet spr = spritesheet_load("data/test.png");
  u32 i = 2;

  while(!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_GREEN);

    if (input_pressed(KEY_SPACE))
      DEBUG("SPACE");

    if (mouse_pressed(MOUSE_LEFT))
      DEBUG("LEFT");
    if (mouse_pressed(MOUSE_RIGHT))
      DEBUG("RIGHT");
    if (mouse_pressed(MOUSE_MIDDLE))
      DEBUG("middle");
    if (mouse_pressed(MOUSE_BUTTON4))
      DEBUG("4");
    if (mouse_pressed(MOUSE_BUTTON5))
      DEBUG("5");

    render_texture(spritesheet_get(spr, i), (vec2){0});

    window_updatelate();
  }

  spritesheet_free(spr);
  window_destroy();

  return 0;
}
