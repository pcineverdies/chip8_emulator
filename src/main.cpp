#include "memory.h"
#include "chip8.h"
#include "keyboard.h"
#include "display.h"

int main(int argc, char* argv[]){

  chip8 cpu;
  Memory dmem(4096);
  Memory vmem(2048);
  Keyboard keyboard;
  Display_chip8 display;
  uint16_t key_pressed;

  vmem.write(2, 1);
  vmem.write(1, 1);
  vmem.write(0, 1);
  display.update(&vmem);
  vmem.write(1, 0);
  display.update(&vmem);

  while(keyboard.read_key() == 0);

}
