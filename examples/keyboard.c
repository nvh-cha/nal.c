#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  vec2 pos;

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_BLACK);

    render_rectangle(pos, (vec2){20, 20}, COLOR_RED);

    if (input_down(KEY_D))
      pos.x += 200 * time_getdelta();
    if (input_down(KEY_A))
      pos.x -= 200 * time_getdelta();
    if (input_down(KEY_W))
      pos.y -= 200 * time_getdelta();
    if (input_down(KEY_S))
      pos.y += 200 * time_getdelta();

    DEBUG("%f", time_getdelta());

    window_updatelate();
  }

  window_destroy();
  return 0;
}
