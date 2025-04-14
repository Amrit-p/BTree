#include <stdio.h>
#include "Attribute.h"
#include "Table.h"
#include "array.h"
#include "Value.h"
#include "Record.h"
#include "helper.h"
#include "Tree.h"
#include <string.h>


int main()
{
#ifdef READ
    Table *table = table_read_file("users"TABLE_SUFIX);
#else
    Attribute *id = init_attribute("id", ATTR_INT);
    Attribute *name = init_attribute("name", ATTR_CHAR);
    Attribute *email = init_attribute("email", ATTR_CHAR);
    Attribute *created_at = init_attribute("created_at", ATTR_CHAR);

    Attributes attributes = {0};
    array_push(&attributes, id);
    array_push(&attributes, name);
    array_push(&attributes, email);
    array_push(&attributes, created_at);

    Table *table = init_table("users", &attributes);
#endif

    Record record = {0};
    array_push(&record, INT_VAL(1));
    array_push(&record, CHAR_VAL("amrit"));
    array_push(&record, CHAR_VAL("amritpal6239@gmail.com"));
    array_push(&record, CHAR_VAL("02-02-2025"));
    if (table == NULL)
    {
        array_free(&record);
#ifndef READ
        array_free(&attributes);
#endif

        return 1;
    }
    printf("pager->offset = %zu\n", table->pager->offset);
    table_insert(table, &record);
    //table_insert(table, &record);
    table_dump(table);

    // for (uint32_t i = 0; i < table->record_count; i++)
    // {

    //     Record row = {0};

    //     void *page = table_record_slot(table, i);
    //     void *cursor = page;

    //     for (size_t j = 0; j < array_size(table->attributes); j++)
    //     {
    //         Attribute *attr = array_at(table->attributes, j);
    //         switch (attr->type)
    //         {
    //         case ATTR_INT:
    //         {
    //             DB_INT x;
    //             memcpy(&x, cursor, attribute_size(ATTR_INT));
    //             cursor = (void *)((size_t)cursor + attribute_size(ATTR_INT));
    //             printf("x %d\n",x);
    //             array_push(&row, INT_VAL(x));
    //         }
    //         break;
    //         case ATTR_CHAR:
    //         {
    //             uint8_t prefix_len[CHAR_PREFIX_LEN] = {0};
    //             memcpy(prefix_len, cursor, CHAR_PREFIX_LEN);
    //             uint8_t len = prefix_len[0];
    //             printf("prefix len \"%d\" cursor \"%p\" page \"%p\"\n", len, cursor, page);

    //             cursor = (void *)((size_t)cursor + CHAR_PREFIX_LEN);
    //             DB_CHAR x[DB_VARHCAR_LEN] = {0};
    //             memcpy(x, cursor, len);
    //             printf("%s \"%s\"\n", attr->name, x);

    //             cursor = (void *)((size_t)cursor + len);
    //             Value value = CHAR_VAL(0);
    //             strcpy(AS_CHAR(value), x);
    //             array_push(&row, value);
    //         }
    //         break;
    //         default:
    //             NOTREACHABLE;
    //         }
    //     }
    //     record_dump(row);
    //     array_free(&row);
    // }
    table_free(table);
#ifndef READ
    array_free(&attributes);
#endif
    array_free(&record);
    return 0;
}