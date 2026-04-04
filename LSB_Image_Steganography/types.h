#ifndef TYPES_H
#define TYPES_H
#include<stdio.h>
typedef unsigned int uint;
typedef enum
{
    success,
    fail
} Status;
typedef enum
{
    encode,
    decode,
    unsupported
} OperationType;
#endif