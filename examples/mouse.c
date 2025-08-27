#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    if (mouse_pressed(MOUSE_LEFT))
      DEBUG("pressed mouse left button");
    if (mouse_pressed(MOUSE_RIGHT))
      DEBUG("pressed mouse right button");
    if (mouse_pressed(MOUSE_MIDDLE))
      DEBUG("pressed mouse scrollwheel");
    if (mouse_pressed(MOUSE_BUTTON4))
      DEBUG("pressed mouse side button 1");
    if (mouse_pressed(MOUSE_BUTTON5))
      DEBUG("pressed mouse side button 2");

    window_updatelate();
  }

  window_destroy();
  return 0;
}
