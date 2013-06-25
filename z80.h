#ifndef Z80_H
#define Z80_H
#include <stdint.h>

class Z80
{
  private:
    static const uint32_t mem_size = 65536;

    static const uint8_t flag_carry = 0;
    static const uint8_t flag_subtract = 1;
    static const uint8_t flag_parityoverflow = 2;
    static const uint8_t flag_halfcarry = 4;
    static const uint8_t flag_zero = 6;
    static const uint8_t flag_sign = 7;

    /* rf bit meanings:

        0: carry - result did not fit in register
        1: subtract - was last operation a subtraction
        2: parity/overflow - 

    */

    uint8_t ra, rb, rc, rd, re, rf, rh, rl;
    uint16_t sp, pc;
    uint8_t memory[mem_size];

    uint8_t x, y, z;

    bool running;

    typedef void (*decode_func_t)(Z80* cpu, uint8_t opcode);
    static const decode_func_t opcodes[256];  

    uint16_t combine_uint8_to_uint16(uint8_t h, uint8_t l);
    bool is_parity_odd(uint8_t val);
    bool is_bit_set(uint16_t val, uint8_t bit_num);

    void set_flag_bit(uint8_t bit_num, bool set);
    bool get_flag_bit(uint8_t bit_num);

    void flags_update_zero(uint8_t val);
    bool flags_get_zero();
    void flags_update_sign(uint8_t val);
    void flags_update_subtract(bool subtract);

    void r(uint8_t ri, uint8_t val);
    uint8_t r(uint8_t ri);
    uint16_t hl();

    void set_mem(uint16_t addr, uint8_t val);
    uint8_t get_mem(uint16_t addr);

    uint8_t fetch();
    void decode_1b(uint8_t opcode);
    void execute(uint8_t opcode);

    static void instruction_NOP(Z80* cpu, uint8_t opcode);
    static void instruction_HALT(Z80* cpu, uint8_t opcode);
    static void instruction_LDI(Z80* cpu, uint8_t opcode);
    static void instruction_LDR(Z80* cpu, uint8_t opcode);
    static void instruction_INC(Z80* cpu, uint8_t opcode);
    static void instruction_DEC(Z80* cpu, uint8_t opcode);
    static void instruction_JMP(Z80* cpu, uint8_t opcode);
    static void instruction_JP(Z80* cpu, uint8_t opcode);
    static void instruction_ALU(Z80* cpu, uint8_t opcode);

  public:
    Z80();
    void run();
    void set_memory(uint8_t* src, uint16_t size);
    void dump_registers();
};
#endif