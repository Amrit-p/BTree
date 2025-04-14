#ifndef CURSOR_H
#define CURSOR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct Cursor Cursor;
#include "Table.h"

struct Cursor
{
    Table *table;
    size_t page_index;
    size_t cell_index;
    bool end_of_table;
};

Cursor *cursor_init(Table *table, bool end_of_table);
Cursor *cursor_start(Table *table);
Cursor *cursor_find(Table *table, size_t cell_index);
void *cursor_value(Cursor *cursor);
void cursor_advance(Cursor *cursor);
void cursor_free(Cursor *cursor);
#endif