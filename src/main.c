#include <nal.h>

int main(int argc, char *argv[]) {
  WindowOptions opt = windowoptions_default();
  window_createx(opt);

  while(!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(0xff000000);

    window_updatelate();
  }

  window_destroy();

  return 0;
}
