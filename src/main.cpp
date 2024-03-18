#include "memory.h"
#include "chip8.h"
#include "keyboard.h"
#include "display.h"
#include <unistd.h>

int main(int argc, char* argv[]){

  chip8 cpu;
  Memory dmem(4096);
  Memory vmem(256);
  Keyboard keyboard;
  Display_chip8 display;
  uint16_t key_pressed;

  cpu.init();
  dmem.init_sprites();
  dmem.init_from_file(0x200, "rom/brick.ch8");

  while(true){
    key_pressed = keyboard.read_key();
    if(key_pressed == 0xffff) break;

    cpu.step(&dmem, &vmem, key_pressed);
    display.update(&vmem);
    usleep(500);
  }

}
