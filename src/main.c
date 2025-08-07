#include <nal.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  window_createx(windowoptions_default());
  bool running = true;

  while(running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
    }
  }

  window_destroy();

  return 0;
}
