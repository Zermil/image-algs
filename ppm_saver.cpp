#include <cstdio>
#include <cstdlib>
#include <cstdint>

#define UNUSED(x) ((void)(x))

typedef uint8_t u8;
typedef uint32_t u32;

void save_ppm(const char *filename, u32 *data, u32 width, u32 height)
{
    FILE *file = fopen(filename, "wb");
    
    fprintf(file, "P6\n");
    fprintf(file, "%u %u\n", width, height);
    fprintf(file, "255\n");

    for (u32 i = 0; i < width * height; ++i) {
        // 0x AA BB GG RR
        u32 pixel = data[i];
        u8 bytes[3] = {
            (pixel >> (8 * 0)) & 0xFF,
            (pixel >> (8 * 1)) & 0xFF,
            (pixel >> (8 * 2)) & 0xFF
        };
        
        fwrite(bytes, sizeof(bytes), 1, file);
    }
    
    fclose(file);
}

void fill_color(u32 *data, u32 width, u32 height, u32 color)
{
    for (u32 i = 0; i < width * height; ++i) {
        data[i] = color;
    }
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    constexpr u32 width = 16;
    constexpr u32 height = 16;
    u32 data[width * height];

    fill_color(data, width, height, 0xFF181818);
    
    for (u32 row = 0; row < height; ++row) {
        for (u32 col = (row & 1); col < width; col += 2) {
            data[col + (width * row)] = 0xFFFFFFFF;
        }
    }
    
    save_ppm("image.ppm", data, width, height);
    
    return 0;
}
