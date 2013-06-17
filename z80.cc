#include <iostream>
#include "z80.h"
#include "z80_opcodes.h"

using namespace std;

Z80::Z80()
{
  memset(this->memory, 0, sizeof(this->memory));

  this->sp = 0;
  this->pc = 0;
}

uint16_t Z80::combine_uint8_to_uint16(uint8_t h, uint8_t l)
{
  uint16_t ret = h;
  ret <<= 8;
  ret |= l;
  return ret;
}

uint16_t Z80::hl()
{
    return this->combine_uint8_to_uint16(this->rh, this->rl);
}

uint8_t Z80::r(uint8_t ri)
{
  switch(ri)
  {
    case 0:
      return this->rb;
    case 1:
      return this->rc;
    case 2:
      return this->rd;
    case 3:
      return this->re;
    case 4:
      return this->rh;
    case 5:
      return this->rl;
    case 6:
      return this->get_mem(this->hl());
    case 7:
      return this->ra;
    default:
      return -1;
  }
}

void Z80::r(uint8_t ri, uint8_t val)
{
  switch(ri)
  {
    case 0:
      this->rb = val;
      break;
    case 1:
      this->rc = val;
      break;
    case 2:
      this->rd = val;
      break;
    case 3:
      this->re = val;
      break;
    case 4:
      this->rh = val;
      break;
    case 5:
      this->rl = val;
      break;
    case 6:
      this->set_mem(this->hl(), val);
      break;
    case 7:
      this->ra = val;
      break;
  }
}

void Z80::set_mem(uint16_t addr, uint8_t val)
{
  this->memory[addr] = val;
}

uint8_t Z80::get_mem(uint16_t addr)
{
  return this->memory[addr];
}

void Z80::decode_1b(uint8_t opcode)
{
  this->x = (opcode & 0xC0) >> 6;
  this->y = (opcode & 0x38) >> 3;
  this->z = opcode & 0x07;
}

uint8_t Z80::fetch()
{
  return this->get_mem(this->pc++);
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
    this->running = false;
  }
}

void Z80::instruction_NOP(Z80* cpu, uint8_t opcode)
{
  // NOP
}

void Z80::instruction_HALT(Z80* cpu, uint8_t opcode)
{
  // HLT
  cpu->pc--;
}

void Z80::instruction_LDR(Z80* cpu, uint8_t opcode)
{
  // LD r[y], r[z]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->z);
  cpu->r(cpu->y, val);
}

void Z80::instruction_LDI(Z80* cpu, uint8_t opcode)
{
  // LD r[y], n
  cpu->decode_1b(opcode);
  cpu->r(cpu->y, cpu->fetch());
}

void Z80::instruction_INC(Z80* cpu, uint8_t opcode)
{
  // INC r[y]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->y) + 1;
  cpu->r(cpu->y, val);
}

void Z80::instruction_DEC(Z80* cpu, uint8_t opcode)
{
  // INC r[y]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->y) - 1;
  cpu->r(cpu->y, val);
}

void Z80::instruction_JMP(Z80* cpu, uint8_t opcode)
{
  uint8_t l = cpu->fetch();
  uint8_t h = cpu->fetch();
  cpu->pc = cpu->combine_uint8_to_uint16(h, l);
}

void Z80::run()
{
  this->running = true;

  while(this->running)
  {
    uint8_t opcode = this->fetch();
    this->execute(opcode);
    this->dump_registers();
  }
}

void Z80::set_memory(uint8_t* src, uint16_t size)
{
  memcpy(this->memory, src, size);
}

void Z80::dump_registers()
{
  printf("\nA=%u\tB=%u\tC=%u\tD=%u\n", this->ra, this->rb, this->rc, this->rd);
  printf("E=%u\tF=%u\tH=%u\tL=%u\n", this->re, this->rf, this->rh, this->rl);
  printf("PC=%u\tSP=%u\tHL=%u\n\n", this->pc, this->sp, this->memory[this->hl()]);
}