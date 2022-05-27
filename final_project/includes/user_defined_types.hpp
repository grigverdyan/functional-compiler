#ifndef USER_DEFINED_TYPES_H
#define USER_DEFINED_TYPES_H

#include "header.hpp"

typedef enum {
    plus = '+',
    minus = '-',
    division = '/',
    remainder = '%',
    multiply = '*',
    equal = '='
}   E_Operators;

typedef enum {
    left = '(',
    right = ')'
}   E_Bracket;

typedef enum {
    BYTE = 1,
    WORD = 2,
    DWORD = 4,
    QWORD = 8
}   E_Type;

typedef  enum {
	R0, R1, R2, R3, R4, R5, R6, R7, R8,  R9,  R10,
    R11, R12, R13, R14, R15, R16, R17, R18, R19, R20,
    R21, R22, R23, R24, R25, R26, R27, R28, R29, R30,
    R31, undefined 
}   E_Reg;

typedef enum {
	SP, SF, A2, A3, A4, A5, A6, A7
}   E_Ar;

#endif // USER_DEFINED_TYPES_H
