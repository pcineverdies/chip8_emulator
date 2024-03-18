#ifndef __CHIP8_H
#define __CHIP8_H

#include <stdint.h>
#include "memory.h"
#include <ctime>
#include <cstdlib>

class chip8 {

  // General registers
  uint8_t regs[16];

  // Index Register
  uint16_t I;

  // Program Counter
  uint16_t PC;

  // Stack Pointer
  uint8_t SP;

  // Stack
  uint16_t stack[64];

  // Timers
  uint8_t DT;
  uint8_t ST;

  // Instructions
  void instr_00E0_CLS(Memory*);
  void instr_00EE_RET();
  void instr_1nnn_JP(uint16_t);
  void instr_2nnn_CALL(uint16_t);
  void instr_3xkk_SE(uint8_t, uint8_t);
  void instr_4xkk_SNE(uint8_t, uint8_t);
  void instr_5xy0_SE(uint8_t, uint8_t);
  void instr_6xkk_LD(uint8_t, uint8_t);
  void instr_7xkk_ADD(uint8_t, uint8_t);
  void instr_8xy0_LD(uint8_t, uint8_t);
  void instr_8xy1_OR(uint8_t, uint8_t);
  void instr_8xy2_AND(uint8_t, uint8_t);
  void instr_8xy3_XOR(uint8_t, uint8_t);
  void instr_8xy4_ADD(uint8_t, uint8_t);
  void instr_8xy5_SUB(uint8_t, uint8_t);
  void instr_8xy6_SHR(uint8_t, uint8_t);
  void instr_8xy7_SUBN(uint8_t, uint8_t);
  void instr_8xyE_SHL(uint8_t, uint8_t);
  void instr_9xy0_SNE(uint8_t, uint8_t);
  void instr_Annn_LD(uint16_t);
  void instr_Bnnn_JP(uint16_t);
  void instr_Cxkk_RND(uint8_t, uint8_t);
  void instr_Dxyn_DRW(uint8_t, uint8_t, uint8_t, Memory*, Memory*);
  void instr_Ex9E_SKP(uint8_t, uint16_t);
  void instr_ExA1_SKNP(uint8_t, uint16_t);
  void instr_Fx07_LD(uint8_t);
  void instr_Fx0A_LD(uint8_t, uint16_t);
  void instr_Fx15_LD(uint8_t);
  void instr_Fx18_LD(uint8_t);
  void instr_Fx1E_ADD(uint8_t);
  void instr_Fx29_LD(uint8_t);
  void instr_Fx33_LD(uint8_t, Memory*);
  void instr_Fx55_LD(uint8_t, Memory*);
  void instr_Fx65_LD(uint8_t, Memory*);

public:
  void step(Memory*, Memory*, uint16_t);
  void init();
  void regs_dump();
};

#endif // !__CHIP8_H
