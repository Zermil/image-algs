#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <string>

/* 
   ========================================================================
   HEADER SECTION
   ======================================================================== 
*/
#define UNUSED(x) ((void)(x))
#define MAX_CAP 32
#define TEMP_BUFFER_SZ 16

typedef uint32_t u32;
typedef int32_t s32;

struct Hf_Node {
    char character;
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

char *file_read_contents(const char *filename);

int main(int argc, char **argv)
{
    Min_Heap heap = {0};
    
    if (argc > 1) {
        const char *contents = file_read_contents(argv[1]);
        
        Hf_Node node = {0};
        char temp_buffer[TEMP_BUFFER_SZ] = {0};
        u32 buff_idx = 0;
        
        for (u32 i = 0; contents[i] != '\0'; ++i) {
            const char c = contents[i];

            if (c == '\n') {
                temp_buffer[buff_idx] = '\0';
                buff_idx = 0;

                node.frequency = atoi(temp_buffer);
                heap_insert(&heap, node);
            } else if (c == ',') {
                node.character = contents[i - 1];
            } else if (isdigit(c) && buff_idx < TEMP_BUFFER_SZ - 1) {
                temp_buffer[buff_idx] = c;
                buff_idx += 1;
            }
        }

        // EOF
        if (buff_idx != 0) {
            temp_buffer[buff_idx] = '\0';
            buff_idx = 0;

            node.frequency = atoi(temp_buffer);
            heap_insert(&heap, node);
        }
        
        free((void *)contents);
    } else {
        // DEFAULT VALUES WHEN FILE NOT PROVIDED
        heap_insert(&heap, { 'a', 15 });
        heap_insert(&heap, { 'b', 12 });
        heap_insert(&heap, { 'c', 5 });
        heap_insert(&heap, { 'd', 4 });
        heap_insert(&heap, { 'e', 1 });
        heap_insert(&heap, { 'f', 45 });
    }
    
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
    
    if (root->left) huffman_print_codes(root->left, str + "0");
    if (root->right) huffman_print_codes(root->right, str + "1");
}

char *file_read_contents(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (file == 0) {
        fprintf(stderr, "[ERROR]: Could not open a file %s\n", filename);
        exit(1);
    }
    
    fseek(file, 0, SEEK_END);
    s32 size = ftell(file); 
    fseek(file, 0, SEEK_SET);
    
    char *contents = (char *)malloc((sizeof(char) * size) + 1);
    
    fread(contents, sizeof(char), size, file);
    contents[size] = '\0';
    
    fclose(file);
    
    return(contents);
}
