#include <iostream>


#include "z80.h"

using namespace std;

int main()
{
  Z80 cpu;
  uint8_t program[] = {0x3C, 0xC3, 0x00};
  cpu.set_memory(program, 2);
  cpu.run();
  return 0;
}