#include <iostream>
#include <stdint.h>

using namespace std;

class Z80
{
  private:
    static const uint32_t mem_size = 65536;

    uint8_t _ra, _rb, _rc, _rd, _re, _rf, _rh, _rl;
    uint8_t _sp, _pc;
    uint8_t _memory[mem_size];

    uint8_t _x, _y, _z;

    bool _running;

    void r(uint8_t ri, uint8_t val);
    uint8_t r(uint8_t ri);

    uint8_t fetch();
    void decode(uint8_t opcode);
    void execute();

  public:
    Z80();
    void run();
    void set_memory(uint16_t dst, uint8_t* src, uint16_t size);
    void dump_registers();
};

Z80::Z80()
{
  memset(this->_memory, 0, sizeof(this->_memory));
  this->_rb = 1;

  this->_sp = 0;
  this->_pc = 0;
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
      // AIN'T NOBODY GOT TIME FOR THAT
      return -1;
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
    case 5:
      this->_rl = val;
    case 6:
      // AIN'T NOBODY GOT TIME FOR THAT
      break;
    case 7:
      this->_ra = val;
  }
}

uint8_t Z80::fetch()
{
  return this->_memory[this->_pc++];
}

void Z80::decode(uint8_t opcode)
{
  this->_x = (opcode & 0xC0) >> 6;
  this->_y = (opcode & 0x38) >> 3;
  this->_z = opcode & 0x07;

  #ifdef DEBUG
    printf("x:%u y:%u z:%u\n", this->_x, this->_y, this->_z);
  #endif
}

void Z80::execute()
{
 if(this->_x == 1 && this->_y == 6 && this->_z == 6)
 {
  this->_running = false;
 }
 else if(this->_x == 1)
 {
  // LD r[y], r[z]
  uint8_t val = this->r(this->_z);
  this->r(this->_y, val);
 }
}

void Z80::run()
{
  this->_running = true;

  while(this->_running)
  {
    uint8_t instruction = this->fetch();
    this->decode(instruction);
    this->execute();
  }
}

void Z80::set_memory(uint16_t dst, uint8_t* src, uint16_t size)
{
  memcpy(&this->_memory[dst], src, size);
}

void Z80::dump_registers()
{
  printf("\nA:%u\tB:%u\tC:%u\tD:%u\n", this->_ra, this->_rb, this->_rc, this->_rd);
  printf("E:%u\tF:%u\tH:%u\tL:%u\n\n", this->_re, this->_rf, this->_rh, this->_rl);
}

int main()
{
  Z80 cpu;
  uint8_t program[] = {0x78, 0x76};
  cpu.set_memory(0, program, 2);
  cpu.dump_registers();
  cpu.run();
  cpu.dump_registers();
  return 0;
}