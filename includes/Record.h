#ifndef RECORD_H
#define RECORD_H
#include "Value.h"

typedef struct
{
    Value *items;
    size_t count;
    size_t capacity;
} Record;

void record_dump(Record *record);
size_t record_size(Record *record);
void record_serialize(Record *record, void *dest);
#endif