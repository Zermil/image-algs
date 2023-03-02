#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>

#define UNUSED(x) ((void)(x))
#define MAX_POINTS 32

typedef uint32_t u32;

/* 
   ========================================================================
   MIN_HEAP SECTION

   PARENT_NODE := (index - 1) / 2;
   LEFT_NODE := (index * 2) + 1;
   RIGHT_NODE := (index * 2) + 2;
   ======================================================================== 
*/
struct Min_Heap {
    u32 data[MAX_POINTS];
    u32 size;
};

void heap_heapify_up(Min_Heap *heap);
void heap_heapify_down(Min_Heap *heap);
void heap_insert(Min_Heap *heap, u32 element);
u32 heap_get_min(Min_Heap *heap);

void heap_heapify_up(Min_Heap *heap)
{
    u32 current_index = (heap->size - 1);
    u32 parent_index = (current_index - 1) / 2;
    
    while (current_index != 0 && heap->data[current_index] < heap->data[parent_index]) {
        // SWAP
        u32 temp = heap->data[parent_index];
        heap->data[parent_index] = heap->data[current_index];
        heap->data[current_index] = temp;
        
        current_index = parent_index;
        parent_index = (current_index - 1) / 2;
    }
}

void heap_heapify_down(Min_Heap *heap)
{
    u32 current_index = 0;
    u32 left_index = (current_index * 2) + 1;
    u32 right_index = (current_index * 2) + 2;
    
    u32 smallest_index = 0;
    
    if (left_index < heap->size && heap->data[left_index] < heap->data[smallest_index]) smallest_index = left_index;
    if (right_index < heap->size && heap->data[right_index] < heap->data[smallest_index]) smallest_index = right_index;

    while (smallest_index != current_index) {
        // SWAP
        u32 temp = heap->data[current_index];
        heap->data[current_index] = heap->data[smallest_index];
        heap->data[smallest_index] = temp;

        // REPEAT AS ABOVE, COULD ALSO BE A RECURSIVE CALL
        current_index = smallest_index;
        left_index = (current_index * 2) + 1;
        right_index = (current_index * 2) + 2;

        if (left_index < heap->size && heap->data[left_index] < heap->data[smallest_index]) smallest_index = left_index;
        if (right_index < heap->size && heap->data[right_index] < heap->data[smallest_index]) smallest_index = right_index;
    }
}

void heap_insert(Min_Heap *heap, u32 element)
{
    assert(heap->size < MAX_POINTS);

    heap->data[heap->size] = element;
    heap->size += 1;

    heap_heapify_up(heap);
}

u32 heap_get_min(Min_Heap *heap)
{
    assert(heap->size > 0);
    
    u32 value = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size -= 1;

    heap_heapify_down(heap);
    
    return(value);
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    Min_Heap heap = {0};

    heap_insert(&heap, 2);
    heap_insert(&heap, 15);
    heap_insert(&heap, 5);
    heap_insert(&heap, 4);
    heap_insert(&heap, 1);
    heap_insert(&heap, 45);
    
    for (u32 i = 0; i < heap.size; ++i) {
        printf("%u\n", heap.data[i]);

        u32 left = (i * 2) + 1;
        u32 right = (i * 2) + 2;
        
        if (left < heap.size) printf("  %u\n", heap.data[left]);
        if (right < heap.size) printf("  %u\n", heap.data[right]);
    }

    printf("===================\n");

    while (heap.size > 0) {
        printf("%u\n", heap_get_min(&heap));
    }
    
    return 0;
}
