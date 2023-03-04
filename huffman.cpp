#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <string>

#define UNUSED(x) ((void)(x))
#define MAX_CAP 32

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

    Hf_Node *left;
    Hf_Node *right;
};

struct Min_Heap {
    Hf_Node *data[MAX_CAP];
    u32 size;

    Hf_Node nodes[MAX_CAP];
    u32 nodes_size;
};

void heap_heapify_up(Min_Heap *heap);
void heap_heapify_down(Min_Heap *heap);
void heap_insert(Min_Heap *heap, Hf_Node element);
Hf_Node *heap_get_min(Min_Heap *heap);

void huffman_print_tree(Hf_Node *root, u32 indent);
void huffman_print_codes(Hf_Node *root, std::string str);
    
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
    
    while (current_index != 0 && heap->data[current_index]->frequency < heap->data[parent_index]->frequency) {
        // SWAP
        Hf_Node *temp = heap->data[parent_index];
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
    
    if (left_index < heap->size && heap->data[left_index]->frequency < heap->data[smallest_index]->frequency) {
        smallest_index = left_index;
    }
    
    if (right_index < heap->size && heap->data[right_index]->frequency < heap->data[smallest_index]->frequency) {
        smallest_index = right_index;
    }

    while (smallest_index != current_index) {
        // SWAP
        Hf_Node *temp = heap->data[current_index];
        heap->data[current_index] = heap->data[smallest_index];
        heap->data[smallest_index] = temp;

        // REPEAT AS ABOVE, COULD ALSO BE A MODIFIED RECURSIVE CALL
        // heap_heapify_down(heap, smallest_index);
        current_index = smallest_index;
        left_index = (current_index * 2) + 1;
        right_index = (current_index * 2) + 2;

        if (left_index < heap->size && heap->data[left_index]->frequency < heap->data[smallest_index]->frequency) {
            smallest_index = left_index;
        }
        
        if (right_index < heap->size && heap->data[right_index]->frequency < heap->data[smallest_index]->frequency) {
            smallest_index = right_index;
        }
    }
}

void heap_insert(Min_Heap *heap, Hf_Node element)
{
    assert(heap->nodes_size < MAX_CAP);

    heap->nodes[heap->nodes_size] = element;
    heap->nodes_size += 1;

    heap->data[heap->size] = &(heap->nodes[heap->nodes_size - 1]);
    heap->size += 1;    
    
    heap_heapify_up(heap);
}

Hf_Node *heap_get_min(Min_Heap *heap)
{
    assert(heap->size > 0);
    
    Hf_Node *value = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size -= 1;

    heap_heapify_down(heap);
    
    return(value);
}

void huffman_print_tree(Hf_Node *root, u32 indent)
{
    for (u32 i = 0; i < indent; ++i) printf(" ");

    if (root->character) printf("(%c, %d)\n", root->character, root->frequency);
    else printf("(%d)\n", root->frequency);

    if (root->left) huffman_print_tree(root->left, indent + 2);
    if (root->right) huffman_print_tree(root->right, indent + 2);
}

void huffman_print_codes(Hf_Node *root, std::string str)
{
    if (root->character != '\0') printf("%c - %s\n", root->character, str.c_str());
    
    if (root->left) huffman_print_codes(root->left, str + "1");
    if (root->right) huffman_print_codes(root->right, str + "0");
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    Min_Heap heap = {0};

    // NULLPTR <=> 0
    heap_insert(&heap, { 'a', 15, 0, 0 });
    heap_insert(&heap, { 'b', 12, 0, 0 });
    heap_insert(&heap, { 'c', 5, 0, 0 });
    heap_insert(&heap, { 'd', 4, 0, 0 });
    heap_insert(&heap, { 'e', 1, 0, 0 });
    heap_insert(&heap, { 'f', 45, 0, 0 });
    
    while (heap.size != 1) {
        Hf_Node *left = heap_get_min(&heap);
        Hf_Node *right = heap_get_min(&heap);

        Hf_Node parent = {0};
        parent.character = '\0';
        parent.frequency = left->frequency + right->frequency;
        parent.left = left;
        parent.right = right;
        
        heap_insert(&heap, parent);
    }

    huffman_print_tree(heap.data[0], 0);
    printf("====================\n");
    huffman_print_codes(heap.data[0], "");
    
    return 0;
}
