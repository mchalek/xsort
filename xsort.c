#include <stdlib.h>
#include <stdint.h>

static inline void swap(uint64_t *a, uint64_t *b) {
    uint64_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void xsort(uint64_t *items, int64_t n_items, uint64_t mask) {
    int64_t mid = n_items / 2;

    uint64_t *p_pivot = items + n_items / 2;
    uint64_t pivot = *p_pivot & mask;

    uint64_t *head = items;
    uint64_t *tail = items + n_items - 1;
    while(head < tail) {
        while((*head & mask) < pivot)
            head++;

        while((*tail & mask) > pivot)
            tail--;

        if(head == tail)
            break;

        if(head < p_pivot && tail > p_pivot) {
            swap(head, tail);
            head++;
            tail--;
            continue;
        }

        if(head == p_pivot && tail > p_pivot) {
            swap(p_pivot, p_pivot + 1);
            swap(p_pivot, tail);
            p_pivot++;
            head++;

            continue;
        }

        if(head < p_pivot && tail == p_pivot) {
            swap(p_pivot, p_pivot - 1);
            swap(p_pivot, head);

            p_pivot--;
            tail--;
            continue;
        }
    }

    xsort(items, p_pivot - items - 1, mask);
    xsort(p_pivot + 1, n_items - (p_pivot - items), mask);
}
