#include "display.h"
#include <iostream>


/** Display_chip8::Display_chip8
    Constructor of the class.
    Creates the window and the renderer for SDL2

    The window is 64x32, scaled by an integer factor SCALE_FACTOR.
    In this way, each pixel from chip8 is mapped into a square
    of size SCALE_FACTOR x SCALE_FACTOR

*/
Display_chip8::Display_chip8() {

  // Init SDL
  SDL_Init(SDL_INIT_VIDEO);

  // Create the window and the renderer
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH * SCALE_FACTOR,
                              WINDOW_HEIGHT * SCALE_FACTOR,
                              0, &this->window, &this->renderer);

  // Set init color
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);

  // Clear window
  SDL_RenderClear(this->renderer);

  // Render window
  SDL_RenderPresent(this->renderer);
}

/** Memory::update
    Given a memory of 256 bytes, update the screen with its content.

    @param mem Memory video memory storing the data to show
*/
void Display_chip8::update(Memory* mem){

  // Check that the size of the memory is the expected one
  if(mem->get_size() != WINDOW_WIDTH * WINDOW_HEIGHT / 8){
    throw std::invalid_argument("Memory size is not right for dispaly");
  }

  // Clear the screen
  SDL_RenderClear(this->renderer);

  // For each byte in the memory
  for(int i = 0; i < mem->get_size(); i++){

    // Read the byte
    uint8_t data = mem->read(i);

    // For each bit in the byte
    for(int j = 0; j < 8; j++){

      // Get the corresponding indexes in the screen matrix
      int column = (i * 8 + j) % WINDOW_WIDTH;
      int row    = (i * 8 + j) / WINDOW_WIDTH;

      // Pick the color to use, either black or white depending whether the bit is set
      if(data & (1 << j)) SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
      else                SDL_SetRenderDrawColor(this->renderer,   0,   0,   0, 255);

      // Fill the whole square in the window corresponding to the pixel
      for(int x = 0; x < SCALE_FACTOR; x++){
        for(int y = 0; y < SCALE_FACTOR; y++){
          SDL_RenderDrawPoint(this->renderer,
                              column * SCALE_FACTOR + x,
                              row * SCALE_FACTOR + y);
        }
      }
    }
  }

  // Render modifications
  SDL_RenderPresent(this->renderer);
}

/** Display_chip8::~Display_chip8
    Destroyer of the class.

*/
Display_chip8::~Display_chip8(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
