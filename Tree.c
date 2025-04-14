#include "Tree.h"
#include "helper.h"

#include <stdio.h>
#include <string.h>

/*
    Comman node header layout
*/
const size_t NODE_TYPE_SIZE = sizeof(uint8_t);
const size_t NODE_TYPE_OFFSET = 0;
const size_t IS_ROOT_SIZE = sizeof(uint8_t);
const size_t IS_ROOT_OFFSET = NODE_TYPE_OFFSET + NODE_TYPE_SIZE;
const size_t PARENT_POINTER_SIZE = sizeof(size_t);
const size_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const size_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

/*
    Leaf node header layout
*/
const size_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(size_t);
const size_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const size_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

/*
    Leaf node body layout
*/
const size_t LEAF_NODE_KEY_SIZE = sizeof(primary_key);
const size_t LEAF_NODE_KEY_OFFSET = 0;
const size_t LEAF_NODE_VALUE_SIZE = 127; // TODO: should be an "attributes_size"

const size_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const size_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const size_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;

const size_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

size_t *tree_lnode_cells(void *node)
{
    return (size_t *)((size_t)node + LEAF_NODE_NUM_CELLS_OFFSET);
}

Cell *tree_lnode_cell(void *node, size_t cell_index)
{
    void *cell_offset = (void *)((size_t)node + LEAF_NODE_HEADER_SIZE + cell_index * LEAF_NODE_CELL_SIZE);
    Cell *cell = calloc(1, sizeof(Cell));
    cell->index = cell_index;
    cell->pk = (primary_key *)cell_offset;
    cell->record = (Record *)((size_t)cell_offset + LEAF_NODE_KEY_SIZE);
    return cell;
}
primary_key *tree_lnode_key(void *node, size_t cell_index)
{
    Cell *cell = tree_lnode_cell(node, cell_index);
    primary_key *pk = cell->pk;
    free(cell);
    return pk;
}

Record *tree_lnode_value(void *node, size_t cell_index)
{
    Cell *cell = tree_lnode_cell(node, cell_index);
    Record *record = cell->record;
    free(cell);
    return record;
    return (void *)((size_t)tree_lnode_cell(node, cell_index) + LEAF_NODE_KEY_SIZE);
}
void tree_init_lnode(void *node)
{
    tree_set_node_type(node, NODE_LEAF);
    *tree_lnode_cells(node) = 0;
}

NodeType tree_get_node_type(void *node)
{
    uint8_t value = *((uint8_t *)((size_t)node + NODE_TYPE_OFFSET));
    return (NodeType)value;
}
void tree_set_node_type(void *node, NodeType type)
{
    uint8_t value = (uint8_t)type;
    *((uint8_t *)((size_t)node + NODE_TYPE_OFFSET)) = value;
}
void tree_lnode_insert_cell(void *node, Cell cell)
{
    size_t num_cells = *tree_lnode_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS)
    {
        fprintf(stderr, "Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }
    if (cell.index < num_cells)
    {
        for (size_t i = num_cells; i > cell.index; i--)
        {
            Cell *current_cell = tree_lnode_cell(node, i);
            Cell *previous_cell = tree_lnode_cell(node, i - 1);

            memcpy(current_cell->pk, previous_cell->pk, LEAF_NODE_CELL_SIZE);
            memcpy(current_cell->record, previous_cell->record, LEAF_NODE_CELL_SIZE);
        }
    }

    *(tree_lnode_cells(node)) += 1;
    *(tree_lnode_key(node, cell.index)) = *cell.pk;
    record_serialize(cell.record, tree_lnode_value(node, cell.index));
}

Cell cell_init(primary_key *pk, size_t index, Record *record)
{
    Cell cell = {
        .index = index,
        .pk = pk,
        .record = record};
    return cell;
}