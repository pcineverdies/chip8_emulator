#include "memory.h"
#include "chip8.h"
#include "keyboard.h"

int main(int argc, char* argv[]){

  chip8 cpu;
  Memory dmem(4096);
  Memory vmem(256);
  Keyboard keyboard;
  uint16_t key_pressed;

  cpu.init();

  dmem.write_instruction(0x200, 0xf10a);

  cpu.regs_dump();

  cpu.step(&dmem, &vmem, keyboard.read_key());

  cpu.regs_dump();
}
