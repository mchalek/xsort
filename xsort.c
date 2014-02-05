#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define XSORT 0

static inline void swap(uint64_t *a, uint64_t *b) {
    uint64_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void xsort(uint64_t *items, int64_t n_items, uint64_t mask) {
    if(2 > n_items)
        return;

    uint64_t *p_pivot = items + n_items / 2;
    uint64_t pivot = *p_pivot & mask;

    uint64_t *head = items;
    uint64_t *tail = items + n_items - 1;
    while(head < tail) {
        while((*head & mask) <= pivot && head < p_pivot)
            head++;

        while((*tail & mask) > pivot)
            tail--;

        if(head < p_pivot && tail > p_pivot) {
            swap(head, tail);
            head++;
            tail--;
            continue;
        }

        if(head == p_pivot && tail > p_pivot) {
            swap(p_pivot, p_pivot + 1);
            if(tail != p_pivot + 1)
                swap(p_pivot, tail);
            p_pivot++;
            head++;

            continue;
        }

        if(head < p_pivot && tail == p_pivot) {
            swap(p_pivot, p_pivot - 1);
            if(head != p_pivot - 1)
                swap(p_pivot, head);

            p_pivot--;
            tail--;
            continue;
        }
    }

    xsort(items, p_pivot - items, mask);
    xsort(p_pivot + 1, n_items - (p_pivot - items) - 1, mask);
}

void print_items(uint64_t items[], int N) {
    int i;
    for(i = 0; i < N; i++) {
        if(i)
            printf(",");
        printf("%ld", items[i]);
    }
    printf("\n");
}

#define N 100000000

int cmp(const void *va, const void *vb) {
    uint64_t a = *((uint64_t *) va);
    uint64_t b = *((uint64_t *) vb);
    return (a < b) ? -1 : 1 - (a == b);
}

int main(void) {
    //uint64_t items[] = {6,4,1,7,5,0,2,4,7,5,3,2,1,6,9,1};
    uint64_t poly = (1l << 63) | (1l << 61) | (1l << 33) | (1l << 23) | 1l;
    uint64_t f0 = 0x23afb4ea12aef13al;
    uint64_t *items0 = malloc(N*sizeof(uint64_t));
    uint64_t *items1 = malloc(N*sizeof(uint64_t));
    int i;
    items0[0] = f0;
    items1[0] = f0;
    for(i = 1; i < N; i++) {
        items0[i] = ((items0[i-1] & 1)*poly) ^ (items0[i-1] >> 1);
        items1[i] = items0[i];
    }

    for(i = 0; i < N; i++) {
        if(items0[i] != items1[i]) {
            printf("pre-sort error! %016lx != %016lx\n", items0[i], items1[i]);
        }
    }

    if(!XSORT) {
        qsort(items0, N, sizeof(uint64_t), cmp);
    } else {
        xsort(items1, N, 0xffffffffffffffffl);
    }

    exit(0);
    for(i = 0; i < N; i++) {
        if(items0[i] != items1[i]) {
            printf("post-sort error! %016lx != %016lx\n", items0[i], items1[i]);
        }
    }

    return 0;
}
