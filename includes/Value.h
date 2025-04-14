#ifndef VALUE_H
#define VALUE_H

#include "Attribute.h"
#include <stdint.h>

typedef uint32_t DB_INT;

typedef char DB_CHAR;
#define DB_VARHCAR_LEN 40
#define CHAR_PREFIX_LEN 1

typedef struct
{
    AttrType type;
    union
    {
        DB_INT db_int;
        DB_CHAR db_varchar[DB_VARHCAR_LEN];
    } as;
} Value;

#define INT_VAL(num) ((Value){ATTR_INT, {.db_int = num}})
#define CHAR_VAL(value) ((Value){ATTR_CHAR, {.db_varchar = value}})
#define NULL_VAL ((Value){0})
#define AS_INT(value) ((value).as.db_int)
#define AS_CHAR(value) ((value).as.db_varchar)
void value_dump(Value value);
size_t value_size(Value value);
#endif