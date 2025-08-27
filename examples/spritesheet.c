#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  Spritesheet spr = spritesheet_load("data/test.png");
  u32 i = 0;
  DEBUG("press space to change sprite");

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    render_texture(
      spritesheet_get(spr, i),
      (vec2){0}
    );

    if (input_pressed(KEY_SPACE)) {
      i++;
      if (i == spr.len)
        i = 0;
    }

    window_updatelate();
  }

  spritesheet_free(spr);

  window_destroy();
  return 0;
}
