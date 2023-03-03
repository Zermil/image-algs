#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>

#define UNUSED(x) ((void)(x))
#define MAX_POINTS 32

typedef uint8_t u8;
typedef uint32_t u32;

/* 
   ========================================================================
   HEADER SECTION
   ======================================================================== 
*/
struct Hf_Node {
    u8 character;
    u32 frequency;
};

struct Min_Heap {
    Hf_Node data[MAX_POINTS];
    u32 size;
};

void heap_heapify_up(Min_Heap *heap);
void heap_heapify_down(Min_Heap *heap);
void heap_insert(Min_Heap *heap, Hf_Node element);
Hf_Node heap_get_min(Min_Heap *heap);

/* 
   ========================================================================
   IMPLEMENTATION SECTION
   
   PARENT_NODE := (index - 1) / 2;
   LEFT_NODE := (index * 2) + 1;
   RIGHT_NODE := (index * 2) + 2;
   ======================================================================== 
*/
void heap_heapify_up(Min_Heap *heap)
{
    u32 current_index = (heap->size - 1);
    u32 parent_index = (current_index - 1) / 2;
    
    while (current_index != 0 && heap->data[current_index].frequency < heap->data[parent_index].frequency) {
        // SWAP
        Hf_Node temp = heap->data[parent_index];
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
    
    if (left_index < heap->size && heap->data[left_index].frequency < heap->data[smallest_index].frequency) {
        smallest_index = left_index;
    }
    
    if (right_index < heap->size && heap->data[right_index].frequency < heap->data[smallest_index].frequency) {
        smallest_index = right_index;
    }

    while (smallest_index != current_index) {
        // SWAP
        Hf_Node temp = heap->data[current_index];
        heap->data[current_index] = heap->data[smallest_index];
        heap->data[smallest_index] = temp;

        // REPEAT AS ABOVE, COULD ALSO BE A MODIFIED RECURSIVE CALL
        // heap_heapify_down(heap, smallest_index);
        current_index = smallest_index;
        left_index = (current_index * 2) + 1;
        right_index = (current_index * 2) + 2;

        if (left_index < heap->size && heap->data[left_index].frequency < heap->data[smallest_index].frequency) {
            smallest_index = left_index;
        }
        
        if (right_index < heap->size && heap->data[right_index].frequency < heap->data[smallest_index].frequency) {
            smallest_index = right_index;
        }
    }
}

void heap_insert(Min_Heap *heap, Hf_Node element)
{
    assert(heap->size < MAX_POINTS);

    heap->data[heap->size] = element;
    heap->size += 1;

    heap_heapify_up(heap);
}

Hf_Node heap_get_min(Min_Heap *heap)
{
    assert(heap->size > 0);
    
    Hf_Node value = heap->data[0];
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

    heap_insert(&heap, { 'a', 15 });
    heap_insert(&heap, { 'b', 2 });
    heap_insert(&heap, { 'c', 5 });
    heap_insert(&heap, { 'd', 4 });
    heap_insert(&heap, { 'e', 1 });
    heap_insert(&heap, { 'f', 45 });
    
    for (u32 i = 0; i < heap.size; ++i) {
        printf("(%c, %u)\n", heap.data[i].character, heap.data[i].frequency);

        u32 left = (i * 2) + 1;
        if (left < heap.size) printf("  (%c, %u)\n", heap.data[left].character, heap.data[left].frequency);
        
        u32 right = (i * 2) + 2;
        if (right < heap.size) printf("  (%c, %u)\n", heap.data[right].character, heap.data[right].frequency);
    }

    printf("===================\n");

    while (heap.size > 0) {
        Hf_Node node = heap_get_min(&heap);
        printf("(%c, %u)\n", node.character, node.frequency);
    }
    
    return 0;
}
