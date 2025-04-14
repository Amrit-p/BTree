#ifndef TREE_H
#define TREE_H
#include <stdint.h>
#include "Pager.h"
#include "Record.h"
#include "Cursor.h"

typedef enum
{
    NODE_INTERNAL,
    NODE_LEAF,
} NodeType;
typedef size_t primary_key;
typedef struct Cell Cell;

struct Cell
{
    size_t index;
    primary_key *pk;
    Record *record;
};

size_t *tree_lnode_cells(void *node);
Cell *tree_lnode_cell(void *node, size_t cell_index);
primary_key *tree_lnode_key(void *node, size_t cell_index);
Record *tree_lnode_value(void *node, size_t cell_index);
void tree_init_lnode(void *node);
void tree_lnode_insert_cell(void *node, Cell cell);
NodeType tree_get_node_type(void *node);
void tree_set_node_type(void *node, NodeType type);
Cell cell_init(primary_key *pk, size_t index, Record *record);
#endif
