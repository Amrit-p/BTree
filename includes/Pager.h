#ifndef PAGER_H
#define PAGER_H
#define MAX_PAGES 100
#define PAGE_SIZE 4096

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
typedef struct
{
    int fd;
    uint32_t file_length;
    void *pages[MAX_PAGES];
    size_t num_pages;
    off_t offset;
} Pager;
Pager *init_pager(int fd, off_t offset);
void *pager_get_page(Pager *pager, size_t page_index);
void pager_flush(Pager *pager, size_t page_index);
void pager_free(Pager *pager);
#endif