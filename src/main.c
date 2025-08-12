#include <nal.h>
#include <SDL2/SDL_mixer.h>

//TODO WRITE A BUNCH OF INFO

int main(int argc, char *argv[]) {
  WindowOptions opt = windowoptions_default();
  window_createx(opt);

  Spritesheet spr = spritesheet_load("data/test.png");
  u32 i = 0;

  while(!window_shouldclose()) {
    window_update();

    render_changetarget(NULL);
    render_clear(0xff000000);

    if (input_pressed(KEY_SPACE))
      i++;
    render_texture(spritesheet_get(spr, i), (vec2){0});

    window_updatelate();
  }

  spritesheet_free(spr);
  window_destroy();

  return 0;
}
