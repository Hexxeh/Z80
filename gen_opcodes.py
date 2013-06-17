opcodes = {}

for n in xrange(0x00, 0x100):
  opcode = '0'

  x = (n & 0xC0) >> 6;
  y = (n & 0x38) >> 3;
  z = n & 0x07;

  if x == 0:
    if z == 0:
      if y == 0:
        opcode = 'NOP'
    if z == 4:
      opcode = 'INC'
    if z == 5:
      opcode = 'DEC'
    if z == 6:
      opcode = 'LDI'
  if x == 1:
    opcode = 'LDR'

    if z == 6 and y == 6:
      opcode = 'HALT'
  if x == 3:
    if z == 2:
      opcode = 'JP'
    if z == 3:
      if y == 0:
        opcode = 'JMP'

  opcodes[n] = 'instruction_'+opcode if opcode != '0' else '0'

opcode_str = ', '.join(opcodes.values())
print '#ifndef Z80_OPCODES_H'
print '#define Z80_OPCODES_H'
print 'const Z80::decode_func_t Z80::opcodes[256] = { %s };' % opcode_str
print '#endif'