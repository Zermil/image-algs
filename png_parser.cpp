#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#define UNUSED(x) ((void)(x))

typedef unsigned char u8;
typedef uint32_t u32;
typedef int32_t s32;

#define EXIT_ERROR(cond, msg, ...)                  \
    do {                                            \
        if ((cond)) {                               \
            fprintf(stderr, (msg), __VA_ARGS__);    \
            exit(1);                                \
        }                                           \
    } while(0)

#define SIGNATURE_SIZE 8
#define PNG_HEADER_SIZE 13
#define CHUNK_TYPE_SIZE 4

struct PNG_File {
    const u8 *data;
    s32 size;
    s32 cursor;
};
PNG_File png_open_file(const char *filename);
void png_close_file(PNG_File *png);

struct PNG_Header {
    u32 width;
    u32 height;
    u8 bit_depth;
    u8 color_type;
    u8 compression;
    u8 filter;
    u8 interlace;
};
PNG_Header png_parse_header(PNG_File *png);

template<typename T> T read_bytes(PNG_File *png);
void read_bytes_to_array(PNG_File *png, u8 *array, u32 size);

int main(int argc, char **argv)
{
    PNG_File png = {0};
    if (argc > 1) png = png_open_file(argv[1]);
    else png = png_open_file("sample.png");

    PNG_Header png_header = png_parse_header(&png);

    printf("Dimensions: %ux%u\n", png_header.width, png_header.height);
    printf("Depth: %u\n", png_header.bit_depth);
    printf("Color type: %u\n", png_header.color_type);
    printf("Compression type: %u\n", png_header.compression);
    printf("Filter type: %u\n", png_header.filter);
    printf("Interlace method: %u\n", png_header.interlace);

    png_close_file(&png);
    
    return 0;
}

PNG_File png_open_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    EXIT_ERROR(file == 0, "[ERROR]: Could not open file: %s\n", filename);

    fseek(file, 0, SEEK_END);
    s32 size = ftell(file);
    fseek(file, 0, SEEK_SET);

    u8 *contents = (u8 *) malloc(sizeof(u8) * size);
    fread(contents, sizeof(u8), size, file);
    
    fclose(file);

    PNG_File png = {0};
    png.data = contents;
    png.size = size;
    png.cursor = 0;

    return(png);
}

void png_close_file(PNG_File *png)
{
    png->size = 0;
    png->cursor = 0;
    
    if (png->data != 0) free((void *) png->data);
}

template<typename T>
T read_bytes(PNG_File *png)
{
    T value = 0;
    u32 size = sizeof(T);

    for (u32 i = 0; i < size; ++i) {
        value = (value << 8) | png->data[png->cursor];
        png->cursor += 1;
    }
    
    return(value);
}

void read_bytes_to_array(PNG_File *png, u8 *array, u32 size)
{
    for (u32 i = 0; i < size; ++i) {
        array[i] = png->data[png->cursor];
        png->cursor += 1;
    }
}

PNG_Header png_parse_header(PNG_File *png)
{
    const u8 signature[SIGNATURE_SIZE] = {137, 80, 78, 71, 13, 10, 26, 10};
    PNG_Header header = {0};

    for (u32 i = 0; i < SIGNATURE_SIZE; ++i) {
        EXIT_ERROR(png->data[png->cursor] != signature[i], "[ERROR]: PNG file contains invalid signature!\n");
        png->cursor += 1;
    }
    
    EXIT_ERROR(read_bytes<u32>(png) != PNG_HEADER_SIZE, "[ERROR]: Invalid PNG header size\n");

    u8 chunk_type[CHUNK_TYPE_SIZE + 1] = {0};
    read_bytes_to_array(png, chunk_type, CHUNK_TYPE_SIZE);
    EXIT_ERROR(strncmp((const char *) chunk_type, "IHDR", CHUNK_TYPE_SIZE) != 0, "[ERROR]: Invalid PNG header identifier\n");
    
    header.width = read_bytes<u32>(png);
    header.height = read_bytes<u32>(png);
    header.bit_depth = read_bytes<u8>(png);
    header.color_type = read_bytes<u8>(png);
    header.compression = read_bytes<u8>(png);
    header.filter = read_bytes<u8>(png);
    header.interlace = read_bytes<u8>(png);
    
    return(header);
}
