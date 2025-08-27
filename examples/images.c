#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  Texture image = texture_load("data/test.png)");

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    render_texture(image, (vec2){400, 300});

    window_updatelate();
  }

  texture_free(&image);
  window_destroy();
  return 0;
}
