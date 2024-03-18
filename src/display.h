#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "memory.h"
#include <stdexcept>

#define WINDOW_WIDTH  64
#define WINDOW_HEIGHT 32
#define SCALE_FACTOR  8

class Display_chip8{
  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;

public:
        Display_chip8();
  void  update(Memory*);
        ~Display_chip8();
};

#endif // !__DISPLAY_H
