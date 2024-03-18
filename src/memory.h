#ifndef __MEMORY_H
#define __MEMORY_H

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <fstream>
#include <cstddef>
#include <iostream>

class Memory {
  uint32_t size;
  std::vector<uint8_t> memory;

public:
            Memory(uint32_t);
  uint8_t   read(uint16_t);
  void      write(uint16_t, uint8_t);
  void      write_instruction(uint16_t, uint16_t);
  void      init_sprites();
  uint32_t  get_size();
  void      init_from_file(uint16_t, std::string);
};

#endif // !__MEMORY_H
