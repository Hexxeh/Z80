#include <iostream>


#include "z80.h"

using namespace std;

int main()
{
  Z80 cpu;
  uint8_t program[] = {0x3C, 0x76};
  cpu.set_memory(0, program, 2);
  cpu.dump_registers();
  cpu.run();
  cpu.dump_registers();
  return 0;
}