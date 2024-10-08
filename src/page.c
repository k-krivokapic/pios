#include <stdio.h>
#include "page.h"

struct ppage physical_page_array[128]; // 128 pages

struct ppage *free_list = NULL;  // head of list

void init_pfa_list(void) {
    unsigned long base_address = 0x0;

    // traverse the physical page array
    for (int i = 0; i < 128; i++) {
        physical_page_array[i].physical_addr = (void *)(base_address + i * (2 * 1024 * 1024));  // 2MB pages

        // link this page into the free list
        physical_page_array[i].next = free_list;
        physical_page_array[i].prev = NULL;

        // if the list is not empty, update the previous pointer of the old head
        if (free_list != NULL) {
            free_list->prev = &physical_page_array[i];
        }

        free_list = &physical_page_array[i];
    }
}

struct ppage *allocate_physical_pages(unsigned int npages) {
    struct ppage *allocd_list = NULL;  // head of list
    struct ppage *current_page = NULL;

    // traverse the list and allocate the requested number of pages
    for (unsigned int i = 0; i < npages; i++) {
        if (free_list == NULL) {
            // return null if there are not enough free pages
            return NULL;
        }

        // unlink the first page from the list
        current_page = free_list;
        free_list = free_list->next;

        // if there's a next page, update prev
        if (free_list != NULL) {
            free_list->prev = NULL;
        }

        // insert the unlinked page into the list as the head
        current_page->next = allocd_list;
        current_page->prev = NULL;

        if (allocd_list != NULL) {
            allocd_list->prev = current_page;  // updates prev of head
        }

        allocd_list = current_page;  // updates head
    }

    return allocd_list; 
}

void free_physical_pages(struct ppage *ppage_list) {
    struct ppage *current_page = ppage_list;

    // adds each page back into the list
    while (current_page != NULL) {
        struct ppage *next_page = current_page->next; // stores the next page

        // current page becomes new head
        current_page->next = free_list;
        current_page->prev = NULL;

        if (free_list != NULL) {
            free_list->prev = current_page;  // updates the old head's prev pointer
        }

        free_list = current_page; // updates head

        // go onto the next page in the list
        current_page = next_page;
    }
}
