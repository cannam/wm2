
#include <stdio.h>
#include <stdlib.h>

static long ListImpl_best_new_sizes[] = {
    48, 112, 240, 496, 1008, 2032, 4080, 8176,
    16368, 32752, 65520, 131056, 262128, 524272, 1048560,
    2097136, 4194288, 8388592, 16777200, 33554416, 67108848
};

long ListImpl_best_new_count(long count, unsigned int size) {
    for (int i = 0; i < sizeof(ListImpl_best_new_sizes)/sizeof(int); ++i) {
        if (count * size < ListImpl_best_new_sizes[i]) {
            return ListImpl_best_new_sizes[i] / size;
        }
    }
    return count;
}

void ListImpl_range_error(long i) {
    fprintf(stderr, "internal error: list index %ld out of range\n", i);
    abort();
}
