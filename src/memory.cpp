#include "memory.h"

uint8_t Memory::read(uint16_t addr){
  if(addr >= 4096)
    throw std::invalid_argument( "Address cannot be >= 4096" );

  return this->memory[addr];
}

void Memory::write(uint16_t addr, uint8_t data){
  if(addr >= 4096)
    throw std::invalid_argument( "Address cannot be >= 4096" );

  this->memory[addr] = data;
}

Memory::Memory(uint32_t size){
  this->size = size;
  this->memory.resize(size);
  for(int i = 0; i < size; i++) this->memory[i] = 0;
}

void Memory::write_instruction(uint16_t addr, uint16_t data){

  if(addr & 1) {
    throw std::invalid_argument("Instruction address has to be even");
  }

  uint8_t lsb = data & 0xff;
  uint8_t msb = (data & 0xff00) >> 8;

  this->memory[addr] = msb;
  this->memory[addr + 1] = lsb;
}

void Memory::init_sprites(){

  if(this->size < 0x4f){
    throw std::invalid_argument("Memory too small to write sprites into");
  }

  // Sprite 0
  this->memory[0x00] = 0xf0;
  this->memory[0x01] = 0x90;
  this->memory[0x02] = 0x90;
  this->memory[0x03] = 0x90;
  this->memory[0x04] = 0xf0;

  // Sprite 1
  this->memory[0x05] = 0x20;
  this->memory[0x06] = 0x60;
  this->memory[0x07] = 0x20;
  this->memory[0x08] = 0x20;
  this->memory[0x09] = 0x70;

  // Sprite 2
  this->memory[0x0a] = 0xf0;
  this->memory[0x0b] = 0x10;
  this->memory[0x0c] = 0xf0;
  this->memory[0x0d] = 0x80;
  this->memory[0x0e] = 0xf0;

  // Sprite 3
  this->memory[0x0f] = 0xf0;
  this->memory[0x10] = 0x10;
  this->memory[0x11] = 0xf0;
  this->memory[0x12] = 0x10;
  this->memory[0x13] = 0xf0;

  // Sprite 4
  this->memory[0x14] = 0x90;
  this->memory[0x15] = 0x90;
  this->memory[0x16] = 0xf0;
  this->memory[0x17] = 0x10;
  this->memory[0x18] = 0x10;

  // Sprite 5
  this->memory[0x19] = 0xf0;
  this->memory[0x1a] = 0x80;
  this->memory[0x1b] = 0xf0;
  this->memory[0x1c] = 0x10;
  this->memory[0x1d] = 0xf0;

  // Sprite 6
  this->memory[0x1e] = 0xf0;
  this->memory[0x1f] = 0x80;
  this->memory[0x20] = 0xf0;
  this->memory[0x21] = 0x90;
  this->memory[0x22] = 0xf0;

  // Sprite 7
  this->memory[0x23] = 0xf0;
  this->memory[0x24] = 0x10;
  this->memory[0x25] = 0x20;
  this->memory[0x26] = 0x40;
  this->memory[0x27] = 0x40;

  // Sprite 8
  this->memory[0x28] = 0xf0;
  this->memory[0x29] = 0x90;
  this->memory[0x2a] = 0xf0;
  this->memory[0x2b] = 0x90;
  this->memory[0x2c] = 0xf0;

  // Sprite 9
  this->memory[0x2d] = 0xf0;
  this->memory[0x2e] = 0x90;
  this->memory[0x2f] = 0xf0;
  this->memory[0x30] = 0x10;
  this->memory[0x31] = 0xf0;

  // Sprite A
  this->memory[0x32] = 0xf0;
  this->memory[0x33] = 0x90;
  this->memory[0x34] = 0xf0;
  this->memory[0x35] = 0x90;
  this->memory[0x36] = 0x90;

  // Sprite B
  this->memory[0x37] = 0xe0;
  this->memory[0x38] = 0x90;
  this->memory[0x39] = 0xe0;
  this->memory[0x3a] = 0x90;
  this->memory[0x3b] = 0xe0;

  // Sprite C
  this->memory[0x3c] = 0xf0;
  this->memory[0x3d] = 0x80;
  this->memory[0x3e] = 0x80;
  this->memory[0x3f] = 0x80;
  this->memory[0x40] = 0xf0;

  // Sprite D
  this->memory[0x41] = 0xe0;
  this->memory[0x42] = 0x90;
  this->memory[0x43] = 0x90;
  this->memory[0x44] = 0x90;
  this->memory[0x45] = 0xe0;

  // Sprite E
  this->memory[0x46] = 0xf0;
  this->memory[0x47] = 0x80;
  this->memory[0x48] = 0xf0;
  this->memory[0x49] = 0x80;
  this->memory[0x4a] = 0xf0;

  // Sprite F
  this->memory[0x4b] = 0xf0;
  this->memory[0x4c] = 0x80;
  this->memory[0x4d] = 0xf0;
  this->memory[0x4e] = 0x80;
  this->memory[0x4f] = 0x80;
}

uint32_t Memory::get_size(){
  return this->size;
}
