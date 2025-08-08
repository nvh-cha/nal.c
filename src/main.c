#include <nal.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  WindowOptions opt = windowoptions_default();
  window_createx(opt);
  bool running = true;

  Texture foglio = (Texture) {
    .data = malloc(sizeof(Color)*600*600),
    .size = (vec2u){600, 600}
  };

  while(running) {
    window_update();

    render_changetarget(&foglio);
    render_clear(COLOR_WHITE);
  
    for (u32 i=0;i<30;i++) {
      render_line(
        (vec2){0, 30*20},
        (vec2){i*20, 0},
        COLOR_BLACK
      );
    }
    for (u32 i=0;i<30;i++) {
      render_line(
        (vec2){30*20, 0},
        (vec2){0, i*20},
        COLOR_BLACK
      );
    }

    render_changetarget(NULL);
    render_clear(0xff000000);

    render_texture(foglio, (vec2){10, 10});

    if (input_down(KEY_SPACE)) {
      DEBUG("test");
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
    }

    window_updatelate();
  }

  window_destroy();

  return 0;
}
