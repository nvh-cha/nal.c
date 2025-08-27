#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  Animation ani = animation_create("data/test.png", 0.5, true);

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    render_animation(&ani, (vec2){0});

    window_updatelate();
  }

  animation_free(&ani);

  window_destroy();
  return 0;
}
