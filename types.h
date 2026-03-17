#ifndef TYPES_H
#define TYPES_H

/* User defined type */
typedef unsigned int uint;

/* Status type */
typedef enum
{
    e_success,
    e_failure
} Status;

/* Operation type */
typedef enum
{
    e_read,
    e_write,
    e_unsupported
} OperationType;

#endif