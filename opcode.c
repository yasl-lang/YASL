enum opcodes {
ICONST_M1 = 0x02, // push -1 onto stack
ICONST_0  = 0x03, // push 0 onto stack
ICONST_1  = 0x04, // push 1 onto stack
ICONST_2  = 0x05, // push 2 onto stack
ICONST_3  = 0x06, // push 3 onto stack
ICONST_4  = 0x07, // push 4 onto stack
ICONST_5  = 0x08, // push 5 onto stack
ICONST    = 0x09, // push next 8 bytes onto stack as integer constant
DCONST_M1 = 0x0A, // push -1.0 onto stack
DCONST_0  = 0x0B, // push 0.0 onto stack
DCONST_1  = 0x0C, // push 1.0 onto stack
DCONST_2  = 0x0D, // push 2.0 onto stack
DCONST    = 0x0F, // push next 8 bytes onto stack as float constant
NCONST    = 0x20, // push literal nil onto stack
BCONST_F  = 0x21, // push literal false onto stack
BCONST_T  = 0x22, // push literal true onto stack
DUP       = 0x59, // duplicate top value of stack
ADD       = 0x60, // add two integers
SUB       = 0x61, // subtract two integers
MUL       = 0x62, // multiply two integers
DIV       = 0x64, // divide two integers
NEG       = 0x70, // negate an integer
NOT       = 0x71, // negate a boolean
AND       = 0x78, // and, short circuits
OR        = 0x79, // or, short circuits
I2D       = 0x80, // integer to double
D2I       = 0x82, // double to integer
BR        = 0x90, // branch unconditionally
BRF       = 0x91, // branch if condition is falsey
BRT       = 0x92, // branch if condition is truthy
HALT      = 0xF0, // halt
POP       = 0xF1, // pop top of stack
PRINT     = 0xF2, // print top of stack (temporary to allow debugging)
GSTORE    = 0xF4, // store top of stack at addr provided
GLOAD     = 0xF5, // load from addr

};