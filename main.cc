#include <iostream>


#include "z80.h"

using namespace std;

int main()
{
  Z80 cpu;
  uint8_t program[] = {0x3E, 0x0A, 0x3D, 0xC2, 0x02, 0x00, 0x76};
  cpu.set_memory(program, 7);
  cpu.run();
  return 0;
}