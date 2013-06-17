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

bool Z80::is_parity_odd(uint8_t val)
{
  int num_one = 0;
  for(int bit_num = 0; bit_num <= 7; bit_num++)
  {
    if(!!(val | (1 << bit_num))) num_one++;
  }

  return (num_one % 2) != 0;
}

void Z80::set_flag_bit(uint8_t bit_num, bool set)
{
  if(set)
  {
    this->rf = this->rf | (1 << bit_num);
  }
  else
  {
    this->rf = this->rf & ~(1 << bit_num);
  }
}

bool Z80::get_flag_bit(uint8_t bit_num)
{
  return !((this->rf >> bit_num)  & 0x01);
}

void Z80::flags_update_zero(uint8_t val)
{
  this->set_flag_bit(6, val == 0);
}

bool Z80::flags_get_zero()
{
  return this->get_flag_bit(6);
}

void Z80::flags_update_sign(uint8_t val)
{
  this->set_flag_bit(7, !!(val & 0x80));
}

void Z80::flags_update_subtract(bool subtract)
{
  this->set_flag_bit(1, subtract);
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
  cpu->running = false;
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

  cpu->flags_update_sign(val);
  cpu->flags_update_zero(val);
}

void Z80::instruction_DEC(Z80* cpu, uint8_t opcode)
{
  // INC r[y]
  cpu->decode_1b(opcode);
  uint8_t val = cpu->r(cpu->y) - 1;
  cpu->r(cpu->y, val);

  cpu->flags_update_sign(val);
  cpu->flags_update_zero(val);
}

void Z80::instruction_JMP(Z80* cpu, uint8_t opcode)
{
  uint8_t l = cpu->fetch();
  uint8_t h = cpu->fetch();
  cpu->pc = cpu->combine_uint8_to_uint16(h, l);
}

void Z80::instruction_JP(Z80* cpu, uint8_t opcode)
{
  bool condition_true = false;

  cpu->decode_1b(opcode);

  switch(cpu->y)
  {
    case 0:
      // not-zero
      condition_true = cpu->flags_get_zero();
      break;
    case 1:
      // zero
      condition_true = !cpu->flags_get_zero();
      break;
    case 2:
      // no-carry
      break;
    case 3:
      // carry
      break;
    case 4:
      // parity-odd
      break;
    case 5:
      // parity-even
      break;
    case 6:
      // p sign positive?
      break;
    case 7:
      // m sign negative?
      break;
    default:
      break;
  }

  uint8_t l = cpu->fetch();
  uint8_t h = cpu->fetch();
  if(condition_true) cpu->pc = cpu->combine_uint8_to_uint16(h, l);
}

void Z80::run()
{
  this->running = true;

  while(this->running)
  {
    uint8_t opcode = this->fetch();
    this->execute(opcode);
    this->dump_registers();
    sleep(1);
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