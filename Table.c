
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table.h"
#include "array.h"
#include "Value.h"
#include "helper.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Tree.h"

Table *init_table(char *name, Attributes *attributes)
{
    Table *table = calloc(1, sizeof(Table));
    char *buff = calloc(strlen(name) + strlen(TABLE_SUFIX) + 1, sizeof(char));
    strcpy(buff, name);
    strcat(buff, TABLE_SUFIX);
    
    table->name = buff;
    table->attributes = attributes;

    table->attributes_size = attributes_size(attributes);

    int fd = open(table->name, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR);
    if (errno == EEXIST)
    {
        fprintf(stderr, "Error: table \"%s\" already exist.\n", table->name);
        table_free(table);
        return NULL;
    }
    if (fd == -1)
    {
        fprintf(stderr, "Error: failed to open file \"%s\".\nmessage: %s.\n", table->name, strerror(errno));
        table_free(table);
        return NULL;
    }
    table->pager = init_pager(fd, 0);

    // dumping magic constant
    int magic_num = MAGIC_CONSTANT;
    ssize_t bytes_written = write(table->pager->fd, &magic_num, sizeof(int));
    if (bytes_written == -1)
    {
        table_free(table);
        fprintf(stderr, "Error writing: %s\n", strerror(errno));
        return NULL;
    }
    // dumping table name
    size_t table_name_len = strlen(table->name);
    write(table->pager->fd, &table_name_len, sizeof(size_t));
    write(table->pager->fd, table->name, table_name_len);

    // dumping attributes
    write(table->pager->fd, &(array_size(table->attributes)), sizeof(size_t));

    for (size_t i = 0; i < array_size(table->attributes); i++)
    {
        Attribute *attr = array_at(table->attributes, i);

        // dumping attribute type
        write(table->pager->fd, &attr->type, sizeof(AttrType));
        // dumping attribute name
        size_t len = strlen(attr->name);
        write(table->pager->fd, &len, sizeof(size_t));
        write(table->pager->fd, attr->name, len);
    }

    table->pager->offset = lseek(table->pager->fd, 0, SEEK_END);
    table->root_page_index = 0;
    void *node = pager_get_page(table->pager, table->root_page_index);
    tree_init_lnode(node);
    return table;
}

void table_dump_attribute(Table *table)
{
    printf("Table :: %s\n", table->name);
    printf("Name    Type\n");
    for (size_t i = 0; i < array_size(table->attributes); i++)
    {
        Attribute *attr = array_at(table->attributes, i);
        printf("%s    %s\n", attr->name, attribute_type_to_str(attr->type));
    }
}
void table_dump(Table *table)
{
    Pager *pager = table->pager;
    for (size_t i = 0; i < pager->num_pages; i++)
    {
        if (pager->pages[i] == NULL)
            continue;

        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }
}
bool table_insert(Table *table, Record *record)
{
    void *node = pager_get_page(table->pager, table->root_page_index);
    size_t num_cells = (*tree_lnode_cells(node));

    Value val = array_at(record, 0);
    primary_key pk = (primary_key)AS_INT(val);
    Cursor *cursor = cursor_find(table, pk);
    if (cursor->cell_index < num_cells)
    {
        primary_key pk_at_index = *tree_lnode_key(node, cursor->cell_index);
        if (pk_at_index == pk)
        {
            printf("Error: Duplicate entry '%zu' for key 'PRIMARY'\n", pk);
            return false;
        }
    }
    Cell cell = cell_init(&pk, cursor->cell_index, record);
    tree_lnode_insert_cell(node, cell);
    cursor_free(cursor);
    return true;
}
Table *table_read_file(char *file_path)
{
    FILE *fd = fopen(file_path, "rb");
    if (fd == NULL)
    {
        fprintf(stderr, "Error: table \"%s\" does not exist.\n", file_path);
        return NULL;
    }
    int magic_num = 0;
    fread(&magic_num, sizeof(int), 1, fd);

    if (magic_num != MAGIC_CONSTANT)
    {
        fprintf(stderr, "[ERROR] failed to read file \"%s\".\nmessage: cannot find MAGIC_CONSTANT.\n", file_path);
        return NULL;
    }

    size_t table_name_len;
    fread(&table_name_len, sizeof(size_t), 1, fd);
    char *table_name = calloc(table_name_len + 1, sizeof(char));
    fread(table_name, sizeof(char), table_name_len, fd);

    size_t total_attributes;
    fread(&total_attributes, sizeof(size_t), 1, fd);

    size_t i = 0;
    Attributes *attributes = calloc(1, sizeof(Attributes));

    while (i < total_attributes)
    {
        AttrType type;
        fread(&type, sizeof(AttrType), 1, fd);

        size_t len;
        fread(&len, sizeof(size_t), 1, fd);

        char *name = calloc(len + 1, sizeof(char));
        fread(name, sizeof(char), len, fd);
        Attribute *attr = init_attribute(name, type);
        array_push(attributes, attr);
        free(name);
        i++;
    }
    off_t offset = ftell(fd);

    fclose(fd);
    Table *table = calloc(1, sizeof(Table));
    table->name = table_name;
    table->attributes = attributes;

    table->attributes_size = attributes_size(attributes);

    int fd2 = open(table->name, O_RDWR, S_IWUSR | S_IRUSR);

    if (fd2 == -1)
    {
        fprintf(stderr, "Error: failed to open file \"%s\".\nmessage: %s.\n", table->name, strerror(errno));
        table_free(table);
        return NULL;
    }
    table->pager = init_pager(fd2, (off_t)offset);
    void *node = pager_get_page(table->pager, 0);
    tree_init_lnode(node);
    return table;
}

void table_dump_records(Table *table)
{
    (void)table;
    // for (size_t i = 0; i < table->record_count; i++)
    // {
    //     if (i == 0)
    //     {
    //         for (size_t j = 0; j < array_size(table->attributes); j++)
    //         {
    //             Attribute *attr = array_at(table->attributes, j);
    //             printf("%s   ", attr->name);
    //         }
    //         printf("\n");
    //     }
    //     Record record = {0};
    //     record_dump(&record);
    // }
}

void table_free(Table *table)
{
    free(table->name);
    for (size_t i = 0; i < array_size(table->attributes); i++)
    {
        Attribute *attr = array_at(table->attributes, i);
        attribute_free(attr);
    }
    if (table->pager)
        pager_free(table->pager);
    free(table);
}