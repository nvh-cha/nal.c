#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  Tilemap tilemap = tilemap_create("data/tilemap.csv", spritesheet_load("data/test.png"));

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    render_tilemap(tilemap, (vec2){0});

    window_updatelate();
  }

  tilemap_free(&tilemap);

  window_destroy();
  return 0;
}
