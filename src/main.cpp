#include "memory.h"
#include "chip8.h"
#include "keyboard.h"
#include "display.h"

int main(int argc, char* argv[]){

  chip8 cpu;
  Memory dmem(4096);
  Memory vmem(256);
  Keyboard keyboard;
  Display_chip8 display;
  uint16_t key_pressed;

  int cc = 0;
  int max_cc = 1000;

  cpu.init();
  dmem.init_sprites();
  dmem.init_from_file(0x200, "rom/picture.ch8");

  while(cc++ < max_cc){
    cpu.step(&dmem, &vmem, keyboard.read_key());
    display.update(&vmem);
  }

}
