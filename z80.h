#ifndef Z80_H
#define Z80_H
#include <stdint.h>

class Z80
{
  private:
    static const uint32_t mem_size = 65536;

    uint8_t _ra, _rb, _rc, _rd, _re, _rf, _rh, _rl;
    uint8_t _sp, _pc;
    uint8_t _memory[mem_size];

    uint8_t _x, _y, _z;

    bool _running;

    typedef void (*decode_func_t)(Z80* cpu, uint8_t opcode);
    static const decode_func_t opcodes[256];  

    void r(uint8_t ri, uint8_t val);
    uint8_t r(uint8_t ri);
    uint16_t hl();

    uint8_t fetch();
    void decode_1b(uint8_t opcode);
    void execute(uint8_t opcode);

    static void instruction_NOP(Z80* cpu, uint8_t opcode);
    static void instruction_HALT(Z80* cpu, uint8_t opcode);
    static void instruction_LDI(Z80* cpu, uint8_t opcode);
    static void instruction_LDR(Z80* cpu, uint8_t opcode);
    static void instruction_INC(Z80* cpu, uint8_t opcode);
    static void instruction_DEC(Z80* cpu, uint8_t opcode);

  public:
    Z80();
    void run();
    void set_memory(uint16_t dst, uint8_t* src, uint16_t size);
    void dump_registers();
};
#endif