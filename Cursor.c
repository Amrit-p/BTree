#include "Cursor.h"
#include "Pager.h"
#include "Tree.h"
#include "helper.h"

Cursor *cursor_init(Table *table, bool end_of_table)
{
    Cursor *cursor = calloc(1, sizeof(Cursor));
    cursor->table = table;
    cursor->end_of_table = end_of_table;
    cursor->page_index = table->root_page_index;
    return cursor;
}

Cursor *cursor_start(Table *table)
{
    void *root_node = pager_get_page(table->pager, table->root_page_index);
    size_t num_cells = *tree_lnode_cells(root_node);
    Cursor *cursor = cursor_init(table, num_cells == 0);
    return cursor;
}

Cursor *cursor_find(Table *table, size_t pk)
{
    size_t root_page_index = table->root_page_index;
    void *root_node = pager_get_page(table->pager, root_page_index);

    if (tree_get_node_type(root_node) == NODE_LEAF)
    {
        size_t num_cells = *tree_lnode_cells(root_node);
        Cursor *cursor = cursor_init(table, false);
        size_t min_index = 0;
        size_t one_past_max_index = num_cells;
        while (min_index != one_past_max_index)
        {
            size_t index = (min_index + one_past_max_index) / 2;
            primary_key key_at_index = *tree_lnode_key(root_node, index);
            if (key_at_index == pk)
            {
                cursor->cell_index = index;
                return cursor;
            }
            if (pk < key_at_index)
            {
                one_past_max_index = index;
            }
            else
            {
                min_index = index + 1;
            }
        }
        cursor->cell_index = min_index;
        return cursor;
    }
    else
    {
        NOTIMPLEMENTED("searching an internal node");
    }
}

void *cursor_value(Cursor *cursor)
{
    Table *table = cursor->table;
    size_t page_index = cursor->page_index;
    void *page = pager_get_page(table->pager, page_index);
    return tree_lnode_value(page, cursor->cell_index);
}

void cursor_advance(Cursor *cursor)
{
    size_t page_index = cursor->page_index;
    void *node = pager_get_page(cursor->table->pager, page_index);
    cursor->cell_index += 1;

    if (cursor->cell_index >= (*tree_lnode_cells(node)))
    {
        cursor->end_of_table = true;
    }
}

void cursor_free(Cursor *cursor)
{
    free(cursor);
}