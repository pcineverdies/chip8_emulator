#include "chip8.h"
#include <fstream>

/** Chip8::step
    Step for the CPU operation, corresponding to
    a positive clock cycle. It performs the fetch, decode
    and execute stages, according to the expected behavior.

*/
void chip8::step(Memory* mem, Memory* vmem, uint16_t key){

  // ========= fetch stage

  // Fetch 16 bits instruction from memory, msb first
  uint8_t msb_instr = mem->read(this->PC);
  uint8_t lsb_instr = mem->read(this->PC+1);
  uint16_t IR = (msb_instr << 8 | lsb_instr);

  // Increment program counter
  this->PC += 2;

  // Update timers
  if(DT > 0) DT--;
  if(ST > 0) ST--;

  // ========= decode stage

  // bits from 0 to 3
  uint8_t IR_0    = (IR & 0x000f) >> 0;

  // bits from 4 to 7
  uint8_t IR_1    = (IR & 0x00f0) >> 4;

  // bits from 8 to 11
  uint8_t IR_2    = (IR & 0x0f00) >> 8;

  // bits from 12 to 15
  uint8_t IR_3    = (IR & 0xf000) >> 12;

  // bits from 0 to 7
  uint8_t IR_01   = (IR & 0x00ff);

  // bits from 0 to 11
  uint16_t IR_012 = (IR & 0x0fff);

  // ========= execute stage

  if      (IR   == 0x00e0)                 instr_00E0_CLS(vmem);
  else if (IR   == 0x00ee)                 instr_00EE_RET();
  else if (IR_3 == 0x01)                   instr_1nnn_JP(IR_012);
  else if (IR_3 == 0x02)                   instr_2nnn_CALL(IR_012);
  else if (IR_3 == 0x03)                   instr_3xkk_SE(IR_2, IR_01);
  else if (IR_3 == 0x04)                   instr_4xkk_SNE(IR_2, IR_01);
  else if (IR_3 == 0x05 and IR_0 == 0x00)  instr_5xy0_SE(IR_2, IR_1);
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
  // In case no instruction was recognized, an error is thrown
  else throw std::invalid_argument("Instruction received not valid");
}

/** Chip8::instr_00E0_CLS
    Clear the display.

*/
void chip8::instr_00E0_CLS(Memory * vmem){
  for(int i = 0; i < 256; i++) vmem->write(i, 0);
}

/** Chip8::instr_00EE_RET
    Return from a subroutine.

    The interpreter sets the program counter to the
    address at the top of the stack, then subtracts 1
    from the stack pointer.

*/
void chip8::instr_00EE_RET(){
  this->PC = this->stack[this->SP--];
}

/** Chip8::instr_1nnn_JP
    Jump to location nnn.

    The interpreter sets the program counter to nnn.
*/
void chip8::instr_1nnn_JP(uint16_t nnn){
  this->PC = nnn;
}

/** Chip8::instr_2nnn_CALL
    Call subroutine at nnn.

    The interpreter increments the stack pointer,
    then puts the current PC on the top of the stack.
    The PC is then set to nnn.
*/
void chip8::instr_2nnn_CALL(uint16_t nnn){
  this->stack[++this->SP] = this->PC;
  this->PC = nnn;
}

/** Chip8::3instr_3xkk_SE
    Skip next instruction if Vx = kk.

    The interpreter compares register Vx to kk,
    and if they are equal, increments the program
    counter by 2.

*/
void chip8::instr_3xkk_SE(uint8_t x, uint8_t kk){
  if(this->regs[x] == kk) this->PC += 2;
}

/** Chip8::instr_4xkk_SNE
    Skip next instruction if Vx != kk.

    The interpreter compares register Vx to kk,
    and if they are not equal, increments the program
    counter by 2.

*/
void chip8::instr_4xkk_SNE(uint8_t x, uint8_t kk){
  if(this->regs[x] != kk) this->PC += 2;
}

/** Chip8::instr_5xy0_SE
    Skip next instruction if Vx = Vy.

    The interpreter compares register Vx to Vy,
    and if they are equal, increments the program
    counter by 2.

*/
void chip8::instr_5xy0_SE(uint8_t x, uint8_t y){
  if(this->regs[x] == this->regs[y]) this->PC += 2;
}

/** Chip8::instr_6xkk_LD
    Set Vx = kk.

    The interpreter puts the value kk into register Vx.

*/
void chip8::instr_6xkk_LD(uint8_t x, uint8_t kk){
  this->regs[x] = kk;
}

/** Chip8::instr_7xkk_ADD
    Set Vx = Vx + kk.

    Adds the value kk to the value of register Vx,
    then stores the result in Vx.

*/
void chip8::instr_7xkk_ADD(uint8_t x, uint8_t kk){
  this->regs[x] += kk;
}

/** Chip8::instr_8xy0_LD
    Set Vx = Vy.

    Stores the value of register Vy in register Vx.

*/
void chip8::instr_8xy0_LD(uint8_t x, uint8_t y){
  this->regs[x] = this->regs[y];
}

/** Chip8::instr_8xy1_OR
    Set Vx = Vx OR Vy.

    Performs a bitwise OR on the values of Vx and Vy,
    then stores the result in Vx.
    A bitwise OR compares the corrseponding bits
    from two values, and if either bit is 1,
    then the same bit in the result is also 1. Otherwise, it is 0.

*/
void chip8::instr_8xy1_OR(uint8_t x, uint8_t y){
  this->regs[x] |= this->regs[y];
}

/** Chip8::instr_8xy2_AND
    Set Vx = Vx AND Vy.

    Performs a bitwise AND on the values of Vx and Vy,
    then stores the result in Vx.
    A bitwise AND compares the corrseponding bits from two values,
    and if both bits are 1,
    then the same bit in the result is also 1.
    Otherwise, it is 0.

*/
void chip8::instr_8xy2_AND(uint8_t x, uint8_t y){
  this->regs[x] &= this->regs[y];
}

/** Chip8::instr_8xy3_XOR
    Set Vx = Vx XOR Vy.

    Performs a bitwise exclusive OR on the values of Vx and Vy,
    then stores the result in Vx. An exclusive OR compares
    the corrseponding bits from two values,
    and if the bits are not both the same,
    then the corresponding bit in the result is set to 1.
    Otherwise, it is 0.

*/
void chip8::instr_8xy3_XOR(uint8_t x, uint8_t y){
  this->regs[x] ^= this->regs[y];
}

/** Chip8::instr_8xy4_ADD
    Set Vx = Vx + Vy, set VF = carry.

    The values of Vx and Vy are added together.
    If the result is greater than 8 bits (i.e., > 255,)
    VF is set to 1, otherwise 0. Only the lowest 8
    bits of the result are kept, and stored in Vx.

*/
void chip8::instr_8xy4_ADD(uint8_t x, uint8_t y){
  uint16_t extended = this->regs[x] + this->regs[y];
  this->regs[15] = (extended >= 256) ? 1 : 0;
  this->regs[x] = extended & 0x00ff;
}

/** Chip8::instr_8xy5_SUB
    Set Vx = Vx - Vy, set VF = NOT borrow.

    If Vx > Vy, then VF is set to 1, otherwise 0.
    Then Vy is subtracted from Vx, and the results stored in Vx.

*/
void chip8::instr_8xy5_SUB(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[x] > this->regs[y]) ? 1 : 0;
  this->regs[x] -= this->regs[y];
}

/** Chip8::instr_8xy6_SHR
    Set Vx = Vx SHR 1.

    If the least-significant bit of Vx is 1,
    then VF is set to 1, otherwise 0.
    Then Vx is divided by 2.

*/
void chip8::instr_8xy6_SHR(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[x] & 0x01) ? 1 : 0;
  this->regs[x] >>= 1;
}

/** Chip8::instr_8xy7_SUBN
    Set Vx = Vy - Vx, set VF = NOT borrow.

    If Vy > Vx, then VF is set to 1, otherwise 0.
    Then Vx is subtracted from Vy, and the results stored in Vx

*/
void chip8::instr_8xy7_SUBN(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[y] > this->regs[x]) ? 1 : 0;
  this->regs[x] = this->regs[y] - this->regs[x];
}

/** Chip8::instr_8xyE_SHL
    Set Vx = Vx SHL 1.

    If the most-significant bit of Vx is 1,
    then VF is set to 1, otherwise to 0.
    Then Vx is multiplied by 2.

*/
void chip8::instr_8xyE_SHL(uint8_t x, uint8_t y){
  this->regs[15] = (this->regs[x] & 0x80) ? 1 : 0;
  this->regs[x] <<= 1;
}

/** Chip8::instr_9xy0_SNE
    Skip next instruction if Vx != Vy.

    The values of Vx and Vy are compared,
    and if they are not equal,
    the program counter is increased by 2.


*/
void chip8::instr_9xy0_SNE(uint8_t x, uint8_t y){
  if(this->regs[x] != this->regs[y]) this->PC += 2;
}

/** Chip8::instr_Annn_LD
    Set I = nnn.

    The value of register I is set to nnn.

*/
void chip8::instr_Annn_LD(uint16_t nnn){
  this->I = nnn;
}

/** Chip8::instr_Bnnn_JP
    Jump to location nnn + V0.

    The program counter is set to nnn plus the value of V0.

*/
void chip8::instr_Bnnn_JP(uint16_t nnn){
  this->PC = this->regs[0] + nnn;
}

/** Chip8::instr_Cxkk_RND
  Set Vx = random byte AND kk.

  The interpreter generates a random number from 0 to 255,
  which is then ANDed with the value kk.
  The results are stored in Vx.

*/
void chip8::instr_Cxkk_RND(uint8_t x, uint8_t kk){

  uint8_t rand = std::rand() % 256;
  this->regs[x] = rand & kk;
}

/** Chip8::instr_Dxyn_DRW
    Display n-byte sprite starting at memory location I
    at (Vx, Vy), set VF = collision.

    The interpreter reads n bytes from memory,
    starting at the address stored in I.
    These bytes are then displayed as sprites on screen
    at coordinates (Vx, Vy). Sprites are XORed onto the
    existing screen. If this causes any pixels to be erased,
    VF is set to 1, otherwise it is set to 0.
    If the sprite is positioned so part of it is
    outside the coordinates of the display, it wraps
    around to the opposite side of the screen

*/
void chip8::instr_Dxyn_DRW(uint8_t x, uint8_t y, uint8_t n, Memory* mem, Memory* vmem){

  // Read the registers
  uint8_t vx = this->regs[x];
  uint8_t vy = this->regs[y];

  // Flat to check collisions
  uint8_t flag = 0;

  // For each of the n bytes
  for(int i = 0; i < n; i++){

    // Get inital bit in the screen to draw
    uint16_t vmem_addr_bit = (vy + i) * 64 + vx;

    // Read byte to write
    uint8_t mem_data = mem->read(this->I + i);

    // For each bit of the byte, msb first
    for(int j = 7; j >= 0; j--){

      // Get address of the bytes in the video memory
      uint16_t vmem_addr = vmem_addr_bit / 8;

      // Get position of the bit to write in vmem_addr
      uint8_t vmem_data_bit = vmem_addr_bit % 8;

      // Read current value of vmem
      uint8_t vmem_data = vmem->read(vmem_addr);

      // Possibly modify the vmem
      if(mem_data & (1 << j)){
        if(vmem_data & (1 << vmem_data_bit)) flag = 1;
        vmem_data = (vmem_data ^ (1 << vmem_data_bit));
      }

      // Write the data back
      vmem->write(vmem_addr, vmem_data);

      // Next bit
      vmem_addr_bit++;

      // Go at the beginning of the line if overflow
      vmem_addr_bit = (vmem_addr_bit / 64 != (vy + i)) ? vmem_addr_bit - 64 : vmem_addr_bit;
    }
  }

  // Set collistion register
  this->regs[0xf] = (flag) ? 1 : 0;
}

/** Chip8::instr_Ex9E_SKP
    Skip next instruction if key with the value of Vx is pressed.

    Checks the keyboard, and if the key corresponding to
    the value of Vx is currently in the down position,
    PC is increased by 2.

*/
void chip8::instr_Ex9E_SKP(uint8_t x, uint16_t key){
  if(key & (1 << this->regs[x])) this->PC += 2;
}

/** Chip8::instr_ExA1_SKNP
    Skip next instruction if key with the value of Vx is not pressed.

    Checks the keyboard, and if the key corresponding
    to the value of Vx is currently in the up position,
    PC is increased by 2.

*/
void chip8::instr_ExA1_SKNP(uint8_t x, uint16_t key){
  if((~key & (1 << this->regs[x]))) this->PC += 2;
}

/** Chip8::instr_Fx07_LD
    Set Vx = delay timer value.

    The value of DT is placed into Vx.

*/
void chip8::instr_Fx07_LD(uint8_t x){
  this->regs[x] = this->DT;
}

/** Chip8::instr_Fx0A_LD
    Wait for a key press, store the value of the key in Vx.

    All execution stops until a key is pressed,
    then the value of that key is stored in Vx.

*/
void chip8::instr_Fx0A_LD(uint8_t x, uint16_t key){

  // Go back of one step if no key is pressed
  if(key == 0){
    this->PC -= 2, this->DT++, this->ST++;
  }
  else{

    // Get the first key that is pressed, starting from 0 on
    uint16_t mask = 1;
    uint8_t value = 0;

    while(!(key & mask)) mask <<= 1, value++;

    this->regs[x] = value;
  }
}

/** Chip8::instr_Fx15_LD
    Set delay timer = Vx.

    DT is set equal to the value of Vx.

*/
void chip8::instr_Fx15_LD(uint8_t x){
 this->DT = this->regs[x];
}

/** Chip8::instr_Fx18_LD
    Set sound timer = Vx.

    ST is set equal to the value of Vx.

*/
void chip8::instr_Fx18_LD(uint8_t x){
 this->ST = this->regs[x];
}

/** Chip8::instr_Fx1E_ADD
    Set I = I + Vx.

    The values of I and Vx are added, and the results are stored in I.

*/
void chip8::instr_Fx1E_ADD(uint8_t x){
  this->I = this->regs[x] + this->I;
}

/** Chip8::instr_Fx29_LD
    Set I = location of sprite for digit Vx.

    The value of I is set to the location for the
    hexadecimal sprite corresponding to the value of Vx.

*/
void chip8::instr_Fx29_LD(uint8_t x){
  this->I = this->regs[x] * 5;
}

/** Chip8::instr_Fx33_LD
    Store BCD representation of Vx in memory
    locations I, I+1, and I+2.

    The interpreter takes the decimal value of Vx,
    and places the hundreds digit in memory at location in I,
    the tens digit at location I+1, and the ones
    digit at location I+2.

*/
void chip8::instr_Fx33_LD(uint8_t x, Memory* mem){
  uint8_t value = this->I;
  mem->write(this->I + 2, value % 10); value /= 10;
  mem->write(this->I + 1, value % 10); value /= 10;
  mem->write(this->I    , value % 10);
}

/** Chip8::instr_Fx55_LD
    Store registers V0 through Vx in memory starting at location I.

    The interpreter copies the values of registers V0
    through Vx into memory, starting at the address in I.

*/
void chip8::instr_Fx55_LD(uint8_t x, Memory* mem){
  for(int i = 0; i <= x; i++) mem->write(this->I + i, this->regs[i]);
}

/** Chip8::instr_Fx65_LD
    Read registers V0 through Vx from memory starting at location I.

    The interpreter reads values from memory starting at location I
    into registers V0 through Vx.

*/
void chip8::instr_Fx65_LD(uint8_t x, Memory* mem){
  for(int i = 0; i <= x; i++) this->regs[i] = mem->read(this->I + i);
}

/** Chip8::init
    Initilize the elements of the CPU

*/
void chip8::init(){

  srand(time(0));

  // Reset all the registers
  for(int i = 0; i < 16; i++) this->regs[i] = 0;
  this->I = 0;
  this->SP = 0;

  // Set the initial value of PC
  this->PC = 0x200;

  // Reset stack
  for(int i = 0; i < 64; i++) this->stack[i] = 0;

  // Reset timers
  this->DT = 0;
  this->ST = 0;
}
