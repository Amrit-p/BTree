#include "Record.h"
#include "Table.h"
#include "array.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>

void record_dump(Record *record)
{
    for (size_t i = 0; i < array_size(record); i++)
    {
        Value value = array_at(record, i);
        value_dump(value);
        printf("    ");
    }
    printf("\n");
}
size_t record_size(Record *record)
{
    size_t x = 0;
    for (size_t i = 0; i < array_size(record); i++)
    {
        Value value = array_at(record, i);
        x += value_size(value);
    }
    return x;
}
void record_serialize(Record *record, void *dest)
{
    void *offset = dest;
    for (size_t i = 0; i < array_size(record); i++)
    {
        Value value = array_at(record, i);
        switch (value.type)
        {
        case ATTR_INT:
        {
            size_t size = attribute_size(ATTR_INT);
            memcpy(offset, &(AS_INT(value)), size);
            offset = (void *)((size_t)offset + size);
        }
        break;
        case ATTR_CHAR:
        {
            size_t len = value_size(value);

            uint8_t prefix_len[CHAR_PREFIX_LEN] = {0};

            prefix_len[0] = len & 0xFF;

            memcpy(offset, prefix_len, CHAR_PREFIX_LEN);
            offset = (void *)((size_t)offset + CHAR_PREFIX_LEN);

            memcpy(offset, AS_CHAR(value), len);
            offset = (void *)((size_t)offset + len);
        }
        break;
        default:
            NOTREACHABLE;
        }
    }
}