#include <iostream>
#include <fstream>

#include "z80.h"

using namespace std;

int main(int argc, char* argv[])
{

  if(argc < 2)
  {
    cout << "Usage: " << argv[0] << " <binary>" << endl;
    return 1;
  }

  ifstream program_binary(argv[1], ios::in|ios::binary|ios::ate);
  if (program_binary.is_open())
  {
    ifstream::pos_type program_size;
    program_size = program_binary.tellg();
    char* program = new char[program_size];
    program_binary.seekg(0, ios::beg);
    program_binary.read(program, program_size);
    program_binary.close();

    #ifdef DEBUG
    cout << "Program is " << program_size << " bytes long" << endl;
    #endif

    Z80 cpu;
    cpu.set_memory((uint8_t*)program, program_size);
    cpu.run();
  }
  else
  {
    cout << "Unable to open program binary " << argv[1] << endl;
  }

  return 0;
}