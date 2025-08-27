#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    window_updatelate();
  }

  window_destroy();
  return 0;
}
