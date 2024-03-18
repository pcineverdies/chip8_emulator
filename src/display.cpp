#include "display.h"
#include <iostream>

Display_chip8::Display_chip8() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH * SCALE_FACTOR,
                              WINDOW_HEIGHT * SCALE_FACTOR,
                              0, &this->window, &this->renderer);
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
  SDL_RenderClear(this->renderer);
  SDL_RenderPresent(this->renderer);
}

void Display_chip8::update(Memory* mem){

  if(mem->get_size() != WINDOW_WIDTH * WINDOW_HEIGHT / 8){
    throw std::invalid_argument("Memory size is not right for dispaly");
  }

  SDL_RenderClear(this->renderer);

  for(int i = 0; i < mem->get_size(); i++){
    uint8_t data = mem->read(i);

    for(int j = 0; j < 8; j++){

      int column = (i * 8 + j) % WINDOW_WIDTH;
      int row    = (i * 8 + j) / WINDOW_WIDTH;

      if(data & (1 << j)) SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
      else                SDL_SetRenderDrawColor(this->renderer,   0,   0,   0, 255);

      for(int x = 0; x < SCALE_FACTOR; x++){
        for(int y = 0; y < SCALE_FACTOR; y++){
          SDL_RenderDrawPoint(this->renderer,
                              column * SCALE_FACTOR + x,
                              row * SCALE_FACTOR + y);
        }
      }
    }
  }

  SDL_RenderPresent(this->renderer);
}

Display_chip8::~Display_chip8(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
