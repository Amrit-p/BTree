#include "Pager.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

Pager *init_pager(int fd, off_t offset)
{

    Pager *pager = calloc(1, sizeof(Pager));
    pager->fd = fd;
    off_t file_length = lseek(fd, 0, SEEK_END);
    pager->file_length = (uint32_t)file_length - (uint32_t)offset;
    pager->num_pages = (size_t)(pager->file_length / PAGE_SIZE);

    if (pager->file_length % PAGE_SIZE != 0)
    {
        fprintf(stderr, "Db file is not a whole number of pages. Corrupt file.\n");
        return NULL;
    }

    for (size_t i = 0; i < MAX_PAGES; i++)
    {
        pager->pages[i] = NULL;
    }
    pager->offset = (off_t)offset;
    return pager;
}
void pager_flush(Pager *pager, size_t page_no)
{
    off_t offset = lseek(pager->fd, pager->offset + (off_t)page_no * PAGE_SIZE, SEEK_SET);
    if (offset == -1)
    {
        printf("Error seeking: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(pager->fd, pager->pages[page_no], PAGE_SIZE);
    if (bytes_written == -1)
    {
        printf("Error writing: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void *pager_get_page(Pager *pager, size_t page_index)
{
    if (page_index > MAX_PAGES)
    {
        fprintf(stderr, "Error: tried to fetch page number out of bounds. %ld > %d\n", page_index, MAX_PAGES);
        exit(EXIT_FAILURE);
    }
    if (pager->pages[page_index] == NULL)
    {
        // Cache miss. Allocate memory and load from file.
        void *page = calloc(PAGE_SIZE, sizeof(uint8_t));
        uint32_t num_pages = pager->file_length / PAGE_SIZE;
        // We might save a partial page at the end of the file
        if (pager->file_length % PAGE_SIZE)
        {
            num_pages += 1;
        }

        if (page_index <= num_pages)
        {
            lseek(pager->fd, pager->offset + (off_t)page_index * PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(pager->fd, page, PAGE_SIZE);
            if (bytes_read == -1)
            {
                printf("Error: failed to read \"%s\"\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_index] = page;

        if (page_index >= pager->num_pages)
        {
            pager->num_pages = page_index + 1;
        }
    }
    return pager->pages[page_index];
}
void pager_free(Pager *pager)
{
    close(pager->fd);
    for (int i = 0; pager->pages[i]; i++)
    {
        free(pager->pages[i]);
    }
    free(pager);
}