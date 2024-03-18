#ifndef __MEMORY_H
#define __MEMORY_H

#include <cstdint>
#include <stdexcept>
#include <vector>

class Memory {
  uint32_t size;
  std::vector<uint8_t> memory;

public:
  Memory(uint32_t);
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t data);
  void write_instruction(uint16_t add, uint16_t data);
  void init_sprites();
};

#endif // !__MEMORY_H
