#ifndef TABLE_H
#define TABLE_H
#define TABLE_SUFIX ".fr"

#include <stdint.h>

#include "Attribute.h"
#include "Record.h"
#include "Pager.h"
typedef struct Table Table;
#include "Cursor.h"

#define MAGIC_CONSTANT 1037

struct Table
{
    char *name;
    Attributes *attributes;
    size_t attributes_size;
    size_t root_page_index;
    Pager *pager;
};

Table *init_table(char *name, Attributes *attributes);
bool table_insert(Table *table, Record *record);
void table_dump_attribute(Table *table);
void table_dump_records(Table *table);
void table_dump(Table *table);
Table *table_read_file(char *file_path);
void table_free(Table *table);
#endif