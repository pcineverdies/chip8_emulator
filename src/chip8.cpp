#include "chip8.h"
#include <fstream>

void chip8::step(Memory* mem, Memory* vmem, uint16_t key){

  // ========= fetch stage

  // Fetch 16 bits instruction from memory, msb first
  uint8_t msb_instr = mem->read(this->PC);
  uint8_t lsb_instr = mem->read(this->PC+1);
  this->IR = (msb_instr << 8 | lsb_instr);

  // Increment program counter
  this->PC += 2;

  // Update timers
  if(DT > 0) DT--;
  if(ST > 0) ST--;

  // ========= decode stage

  uint8_t IR_0   = (this->IR & 0x000f) >> 0 ;
  uint8_t IR_1   = (this->IR & 0x00f0) >> 4 ;
  uint8_t IR_2   = (this->IR & 0x0f00) >> 8;
  uint8_t IR_3   = (this->IR & 0xf000) >> 12;
  uint8_t IR_01  = (this->IR & 0x00ff);
  uint8_t IR_012 = (this->IR & 0x0fff);

  // ========= execute stage

  printf("Current instr: 0x%04x\n", this->IR);

  if      (this->IR == 0x00e0)             instr_00E0_CLS(vmem);
  else if (this->IR == 0x00ee)             instr_00EE_RET();
  else if (IR_3 == 0x01)                   instr_1nnn_JP(IR_012);
  else if (IR_3 == 0x02)                   instr_2nnn_CALL(IR_012);
  else if (IR_3 == 0x03)                   instr_3xkk_SE(IR_2, IR_01);
  else if (IR_3 == 0x04)                   instr_4xkk_SNE(IR_2, IR_01);
  else if (IR_3 == 0x05 and IR_0 == 0x00)  instr_5xy0_SE(IR_2, IR_01);
  else if (IR_3 == 0x06)                   instr_6xkk_LD(IR_2, IR_01);
  else if (IR_3 == 0x07)                   instr_7xkk_ADD(IR_2, IR_01);
  else if (IR_3 == 0x08 and IR_0 == 0x00)  instr_8xy0_LD(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x01)  instr_8xy1_OR(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x02)  instr_8xy2_AND(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x03)  instr_8xy3_XOR(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x04)  instr_8xy4_ADD(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x05)  instr_8xy5_SUB(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x06)  instr_8xy6_SHR(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x07)  instr_8xy7_SUBN(IR_2, IR_1);
  else if (IR_3 == 0x08 and IR_0 == 0x0e)  instr_8xyE_SHL(IR_2, IR_1);
  else if (IR_3 == 0x09 and IR_0 == 0x00)  instr_9xy0_SNE(IR_2, IR_1);
  else if (IR_3 == 0x0a)                   instr_Annn_LD(IR_012);
  else if (IR_3 == 0x0b)                   instr_Bnnn_JP(IR_012);
  else if (IR_3 == 0x0c)                   instr_Cxkk_RND(IR_2, IR_01);
  else if (IR_3 == 0x0d)                   instr_Dxyn_DRW(IR_2, IR_1, IR_0, mem, vmem);
  else if (IR_3 == 0x0e and IR_01 == 0x9e) instr_Ex9E_SKP(IR_2, key);
  else if (IR_3 == 0x0e and IR_01 == 0xa1) instr_ExA1_SKNP(IR_2, key);
  else if (IR_3 == 0x0f and IR_01 == 0x07) instr_Fx07_LD(IR_2);
  else if (IR_3 == 0x0f and IR_01 == 0x0a) instr_Fx0A_LD(IR_2, key);
  else if (IR_3 == 0x0f and IR_01 == 0x15) instr_Fx15_LD(IR_2);
  else if (IR_3 == 0x0f and IR_01 == 0x18) instr_Fx18_LD(IR_2);
  else if (IR_3 == 0x0f and IR_01 == 0x1e) instr_Fx1E_ADD(IR_2);
  else if (IR_3 == 0x0f and IR_01 == 0x29) instr_Fx29_LD(IR_2);
  else if (IR_3 == 0x0f and IR_01 == 0x33) instr_Fx33_LD(IR_2, mem);
  else if (IR_3 == 0x0f and IR_01 == 0x55) instr_Fx55_LD(IR_2, mem);
  else if (IR_3 == 0x0f and IR_01 == 0x65) instr_Fx65_LD(IR_2, mem);
  else std::cout << "NVI\n"; // throw std::invalid_argument("Instruction received not valid");
}

// Reset frame buffer for memory
void chip8::instr_00E0_CLS(Memory * vmem){
  for(int i = 0; i < 256; i++) vmem->write(i, 0);
}

// Return from subroutine
void chip8::instr_00EE_RET(){
  this->PC = this->stack[this->SP--];
}

// Modify PC with fixed value
void chip8::instr_1nnn_JP(uint16_t nnn){
  this->PC = nnn;
}

// Call subroutine at given address
void chip8::instr_2nnn_CALL(uint16_t nnn){
  this->stack[++this->SP] = this->PC;
  this->PC = nnn;
}

// Skip next if value or Vx is kk
void chip8::instr_3xkk_SE(uint8_t x, uint8_t kk){
  if(this->regs[x] == kk) this->PC += 2;
}

// Skip next if value or Vx is not kk
void chip8::instr_4xkk_SNE(uint8_t x, uint8_t kk){
  if(this->regs[x] != kk) this->PC += 2;
}

// Skip next if value or Vx is equal to Vy
void chip8::instr_5xy0_SE(uint8_t x, uint8_t y){
  if(this->regs[x] == this->regs[y]) this->PC += 2;
}

// Load immediate into Vx
void chip8::instr_6xkk_LD(uint8_t x, uint8_t kk){
  this->regs[x] = kk;
}

// Add immediate to Vx
void chip8::instr_7xkk_ADD(uint8_t x, uint8_t kk){
  this->regs[x] += kk;
}

// Set the value of Vx to Vy
void chip8::instr_8xy0_LD(uint8_t x, uint8_t y){
  this->regs[x] = this->regs[y];
}

// Or the value of Vy to Vx
void chip8::instr_8xy1_OR(uint8_t x, uint8_t y){
  this->regs[x] |= this->regs[y];
}

// And the value of Vy to Vx
void chip8::instr_8xy2_AND(uint8_t x, uint8_t y){
  this->regs[x] &= this->regs[y];
}

// Xor the value of Vy to Vx
void chip8::instr_8xy3_XOR(uint8_t x, uint8_t y){
  this->regs[x] ^= this->regs[y];
}

// Add the value of Vx to Vy, computing carry
void chip8::instr_8xy4_ADD(uint8_t x, uint8_t y){
  uint16_t extended = this->regs[x] + this->regs[y];
  this->regs[15] = (extended >= 256) ? 1 : 0;
  this->regs[x] = extended & 0x00ff;
}

// Sub the value of Vx to Vy, computing borrow
void chip8::instr_8xy5_SUB(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[x] = this->regs[y]) ? 1 : 0;
  this->regs[x] -= this->regs[y];
}

// Shift the value of Vx
void chip8::instr_8xy6_SHR(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[x] & 0x01) ? 1 : 0;
  this->regs[x] >>= 1;
}

// Sub the value of Vy to Vx, store in Vx
void chip8::instr_8xy7_SUBN(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[y] = this->regs[x]) ? 1 : 0;
  this->regs[x] = this->regs[y] - this->regs[x];
}

// Shift the value of Vx
void chip8::instr_8xyE_SHL(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[x] & 0x80) ? 1 : 0;
  this->regs[x] <<= 1;
}

// Skip next instruction if Vx is different from Vy
void chip8::instr_9xy0_SNE(uint8_t x, uint8_t y){
  if(this->regs[x] != this->regs[y]) this->PC += 2;
}

// Load immediate into index register
void chip8::instr_Annn_LD(uint16_t nnn){
  this->I = nnn;
}

// Set the value of the program counter to V0 plus immediate
void chip8::instr_Bnnn_JP(uint16_t nnn){
  this->PC = this->regs[0] + nnn;
}

// Set Vx to a random by bitwise anded with kk
void chip8::instr_Cxkk_RND(uint8_t x, uint8_t kk){

  uint8_t rand = std::rand() % 256;
  this->regs[x] = rand & kk;
}

// Draw on screen
void chip8::instr_Dxyn_DRW(uint8_t x, uint8_t y, uint8_t n, Memory* mem, Memory* vmem){
  uint8_t vx = this->regs[x];
  uint8_t vy = this->regs[y];
  uint8_t flag = 0;
  uint16_t vmem_addr_bit = vy * 64 + vx;

  for(int i = 0; i < n; i++){
    uint8_t mem_data = mem->read(this->I + i);

    for(int j = 0; j < 8; j++){
      vmem_addr_bit = (vmem_addr_bit / 64 != vy) ? vmem_addr_bit - 64 : vmem_addr_bit;
      uint16_t vmem_addr = vmem_addr_bit / 8;
      uint8_t vmem_data_bit = vmem_addr_bit % 8;

      uint8_t vmem_data = vmem->read(vmem_addr);

      if(mem_data & (1 << j)){
        if(vmem_data & (i << vmem_data_bit)) flag = 1;
        vmem_data = (vmem_data ^ (1 << vmem_data_bit));
      }

      vmem_addr_bit++;
    }
  }
}

// Skip if key of Vx is pressed
void chip8::instr_Ex9E_SKP(uint8_t x, uint16_t key){
  if(key & (1 << this->regs[x])) this->PC += 2;
}

// Skip if key of Vx is not pressed
void chip8::instr_ExA1_SKNP(uint8_t x, uint16_t key){
  if((~key & (1 << this->regs[x]))) this->PC += 2;
}

// Read value of DT
void chip8::instr_Fx07_LD(uint8_t x){
  this->regs[x] = this->DT;
}

// Wait until one key is pressed
void chip8::instr_Fx0A_LD(uint8_t x, uint16_t key){

  if(key == 0){
    this->PC -= 2, this->DT++, this->ST++;
  }
  else{

    uint16_t mask = 1;
    uint8_t value = 0;

    while(!(key & mask)) mask <<= 1, value++;

    this->regs[x] = value;
  }
}

// Set value of DT
void chip8::instr_Fx15_LD(uint8_t x){
 this->DT = this->regs[x];
}

// Set value of ST
void chip8::instr_Fx18_LD(uint8_t x){
 this->ST = this->regs[x];
}

// Compute index register as Vx plus I
void chip8::instr_Fx1E_ADD(uint8_t x){
  this->I = this->regs[x] + this->I;
}

// Get address of sprite in memory
void chip8::instr_Fx29_LD(uint8_t x){
  this->I = this->regs[x] * 5;
}

// Load BCD representation of vlaue
void chip8::instr_Fx33_LD(uint8_t x, Memory* mem){
  uint8_t value = this->I;
  mem->write(this->I + 2, value % 10); value /= 10;
  mem->write(this->I + 1, value % 10); value /= 10;
  mem->write(this->I    , value % 10);
}

// Write registers in memory
void chip8::instr_Fx55_LD(uint8_t x, Memory* mem){
  for(int i = 0; i <= x; i++) mem->write(this->I + i, this->regs[i]);
}

// Read registers from memory
void chip8::instr_Fx65_LD(uint8_t x, Memory* mem){
  for(int i = 0; i <= x; i++) this->regs[i] = mem->read(this->I + i);
}

void chip8::init(){

  srand(time(0));

  for(int i = 0; i < 16; i++)
    this->regs[i] = 0;

  this->I = 0;
  this->IR = 0;
  this->PC = 0x200;
  this->SP = 0;

  for(int i = 0; i < 64; i++)
    this->stack[i] = 0;

  this->DT = 0;
  this->ST = 0;
}

void chip8::regs_dump(){
  printf("===========\n");
  for(int i = 0; i < 16; i++) printf("R%d \t -> 0x%x\n", i, this->regs[i]);
  printf("I  \t -> 0x%x\n", this->I);
}
