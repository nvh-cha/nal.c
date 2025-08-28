#include <nal.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default()); 

  BlendMode blend = BLEND_NORMAL;

  while (!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(COLOR_WHITE);
    render_changeblending(blend);

    render_rectangle((vec2){0, 0}, (vec2){64, 64}, 0xbbff0000);
    render_rectangle((vec2){32, 32}, (vec2){64, 64}, 0xbb0000ff);

    if (input_pressed(KEY_SPACE)) {
      if (blend == BLEND_LIGHTEN)
        blend = BLEND_NONE;
      else
        blend++;
    }

    window_updatelate();
  }

  window_destroy();
  return 0;
}
