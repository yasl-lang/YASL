#pragma once

typedef enum Opcode {
	NOP = 0x00, // pass
	NCONST = 0x01, // push literal undef onto stack
	ICONST = 0x02, // push next 8 bytes onto stack as integer constant
	ICONST_M1 = 0x03, // push -1 onto stack
	ICONST_0 = 0x04, // push 0 onto stack
	ICONST_1 = 0x05, // push 1 onto stack
	ICONST_2 = 0x06, // push 2 onto stack
	ICONST_3 = 0x07, // push 3 onto stack
	ICONST_4 = 0x08, // push 4 onto stack
	ICONST_5 = 0x09, // push 5 onto stack
	DCONST = 0x0A, // push next 8 bytes onto stack as float constant
	DCONST_0 = 0x0B, // push 0.0 onto stack
	DCONST_1 = 0x0C, // push 1.0 onto stack
	DCONST_2 = 0x0D, // push 2.0 onto stack
	DCONST_N = 0x0E, // push nan onto stack
	DCONST_I = 0x0F, // push inf onto stack
	BCONST_F = 0x10, // push literal false onto stack
	BCONST_T = 0x11, // push literal true onto stack
	FCONST = 0x12, // push function literal onto stack
	MCONST = 0x13, // push builtin function literal onto stack
	BOR = 0x20, // bitwise or
	BXOR = 0x21, // bitwise xor
	BAND = 0x22, // bitwise and
	BANDNOT = 0x23, // bitwise and, with bitwise not on right operand
	BNOT = 0x24, // bit negation
	BSL = 0x25, // bitwise left shift
	BSR = 0x26, // bitwise right shift
	INITFOR = 0x30, // initialises for-loop in VM
	ENDCOMP = 0x31,
	ENDFOR = 0x32, // end for-loop in VM
	ITER_1 = 0x33, // iterate to next, 1 var
	ITER_2 = 0x35, // iterate to next, 2 var
	END = 0x50, // indicate end of list on stack.
	DUP = 0x58, // duplicate top value of stack
	SWAP = 0x5B, // swap top two elements of the stack
	POP = 0x5F, // pop top of stack
	ADD = 0x60, // add two integers
	SUB = 0x61, // subtract two integers
	MUL = 0x62, // multiply two integers
	EXP = 0x63, // exponentiation
	FDIV = 0x64, // divide two integers
	IDIV = 0x65, // divide two ints (return int)
	MOD = 0x66, // modulo two numbers
	NEG = 0x67, // negate an integer
	POS = 0x68, // positive of a number
	NOT = 0x69, // negate a boolean
	LEN = 0x6A, // get length
	CNCT = 0x6B, // concat two strings or lists
	GT = 0x72, // greater than
	GE = 0x73, // greater than or equal
	EQ = 0x74, // equality
	ID = 0x76, // identity
	SET = 0x80, // sets field.
	GET = 0x81, // gets field.
	BR_4 = 0x90, // branch unconditionally (takes next 4 bytes as jump length)
	BR_8 = 0x91, // branch unconditionally (takes next 8 bytes as jump length)
	BRF_4 = 0x92, // branch if condition is falsey (takes next 4 bytes as jump length)
	BRF_8 = 0x93, // branch if condition is falsey (takes next 8 bytes as jump length)
	BRT_4 = 0x94, // branch if condition is truthy (takes next 4 bytes as jump length)
	BRT_8 = 0x95, // branch if condition is truthy (takes next 8 bytes as jump length)
	BRN_4 = 0x96, // branch if condition is not undef (takes next 4 bytes as jump length)
	BRN_8 = 0x97, // branch if condition is not undef (takes next 8 bytes as jump length)
	NEWSPECIALSTR = 0xBA, // new special string.
	NEWSTR = 0xBB, // make new String and push it onto stack (length (8 bytes), string (length bytes))
	NEWTABLE = 0xBC, // make new HashTable and push it onto stack
	NEWLIST = 0xBD, // make new List and push it onto stack
	HALT = 0xF0, // halt
	GSTORE_1 = 0xF4, // store top of stack at addr provided
	LSTORE_1 = 0xF5, // store top of stack as local at addr
	GLOAD_1 = 0xF6, // load global from addr
	LLOAD_1 = 0xF7, // load local from addr
	INIT_CALL = 0xF8,
	CALL = 0xF9,
	RET = 0xFA, // return from function
	RCALL_8 = 0xFC, // tail recursive function call
	//BCALL_8   = 0xFB, // built-in function call
	//MCALL_8   = 0xFC, // method call
		PRINT = 0xFF, // print
} Opcode;

#define NUM_SPECIAL_STRINGS 32

enum SpecialStrings {
	S___GET,      // __get
	S___SET,      // __set

	S_CLEAR,      // clear
	S_COPY,       // copy

	S_ENDSWITH,   // endswith
	S_EXTEND,     // extend

	S_ISAL,       // isal
	S_ISALNUM,    // isalnum
	S_ISNUM,      // isnum
	S_ISSPACE,    // isspace

	S_JOIN,       // join

	S_KEYS,       // keys

	S_LTRIM,      // ltrim

	S_POP,        // pop
	S_PUSH,       // push

	S_REPEAT,     // repeat
	S_REPLACE,    // replace
	S_REVERSE,    // reverse
	S_RTRIM,      // rtrim

	S_SEARCH,     // search
	S_SLICE,      // slice
	S_SPLIT,      // split
	S_STARTSWITH, // startswith

	S_TOBOOL,     // tobool
	S_TOFLOAT,    // tofloat
	S_TOINT,      // toint
	S_TOLOWER,    // tolower
	S_TOSTR,      // tostr
	S_TOUPPER,    // toupper
	S_TRIM,       // trim

	S_VALUES,     // values
};
