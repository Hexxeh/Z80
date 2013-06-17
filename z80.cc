#include <iostream>
#include "z80.h"
#include "z80_opcodes.h"

using namespace std;

Z80::Z80()
{
  memset(this->_memory, 0, sizeof(this->_memory));

  this->_sp = 0;
  this->_pc = 0;
}

uint16_t Z80::hl()
{
    uint16_t addr = this->_rh;
    addr <<= 8;
    addr |=  this->_rl;
    return addr;
}

uint8_t Z80::r(uint8_t ri)
{
  switch(ri)
  {
    case 0:
      return this->_rb;
    case 1:
      return this->_rc;
    case 2:
      return this->_rd;
    case 3:
      return this->_re;
    case 4:
      return this->_rh;
    case 5:
      return this->_rl;
    case 6:
      return this->_memory[this->hl()];
    case 7:
      return this->_ra;
    default:
      return -1;
  }
}

void Z80::r(uint8_t ri, uint8_t val)
{
  switch(ri)
  {
    case 0:
      this->_rb = val;
      break;
    case 1:
      this->_rc = val;
      break;
    case 2:
      this->_rd = val;
      break;
    case 3:
      this->_re = val;
      break;
    case 4:
      this->_rh = val;
      break;
    case 5:
      this->_rl = val;
      break;
    case 6:
      this->_memory[this->hl()] = val;
      break;
    case 7:
      this->_ra = val;
      break;
  }
}

void Z80::decode_1b(uint8_t opcode)
{
  this->_x = (opcode & 0xC0) >> 6;
  this->_y = (opcode & 0x38) >> 3;
  this->_z = opcode & 0x07;
}

uint8_t Z80::fetch()
{
  return this->_memory[this->_pc++];
}

void Z80::execute(uint8_t opcode)
{
  decode_func_t instruction = this->opcodes[opcode];

  if(instruction != NULL)
  {
    instruction(this, opcode);
  }
  else
  {
    printf("Unrecognised opcode %02X\n", opcode);
    this->_running = false;
  }
}

void Z80::instruction_NOP(Z80* cpu, uint8_t opcode)
{
  // NOP
}

void Z80::instruction_HALT(Z80* cpu, uint8_t opcode)
{
  // HLT
  cpu->_running = false;
}

void Z80::instruction_LDR(Z80* cpu, uint8_t opcode)
{
  // LD r[y], r[z]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->_z);
  cpu->r(cpu->_y, val);
}

void Z80::instruction_LDI(Z80* cpu, uint8_t opcode)
{
  // LD r[y], n
  cpu->decode_1b(opcode);
  cpu->r(cpu->_y, cpu->fetch());
}

void Z80::instruction_INC(Z80* cpu, uint8_t opcode)
{
  // INC r[y]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->_y) + 1;
  cpu->r(cpu->_y, val);
}

void Z80::instruction_DEC(Z80* cpu, uint8_t opcode)
{
  // INC r[y]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->_y) - 1;
  cpu->r(cpu->_y, val);
}

void Z80::run()
{
  this->_running = true;

  while(this->_running)
  {
    uint8_t opcode = this->fetch();
    this->execute(opcode);
  }
}

void Z80::set_memory(uint16_t dst, uint8_t* src, uint16_t size)
{
  memcpy(&this->_memory[dst], src, size);
}

void Z80::dump_registers()
{
  printf("\nA: %u\tB: %u\tC: %u\tD: %u\n", this->_ra, this->_rb, this->_rc, this->_rd);
  printf("E: %u\tF: %u\tH: %u\tL: %u\n", this->_re, this->_rf, this->_rh, this->_rl);
  printf("HL: %u\n\n", this->_memory[this->hl()]);
}