#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float f32;

#define UNUSED(x) ((void)(x))

#define WIDTH 8
#define HEIGHT 8
#define PI 3.14159265358f

#define AT(row, col) ((col) + WIDTH * (row))

void print_block(f32 *block)
{
    for (size_t row = 0; row < HEIGHT; ++row) {
        for (size_t col = 0; col < WIDTH; ++col) {
            printf("%7.2f", block[AT(row, col)]);
        }

        printf("\n");
    }
}

void center_around_zero(f32 *block, f32 *op_block)
{
    for (size_t row = 0; row < HEIGHT; ++row) {
        for (size_t col = 0; col < WIDTH; ++col) {
            op_block[AT(row, col)] = block[AT(row, col)] - 128;
        }
    }
}

// https://en.wikipedia.org/wiki/JPEG#Discrete_cosine_transform
f32 compute_single_uv(size_t u, size_t v, f32 *block)
{
    f32 alpha_u = 1.0f / sqrtf(2.0f);
    f32 alpha_v = 1.0f / sqrtf(2.0f);

    if (u != 0) alpha_u = 1.0f;
    if (v != 0) alpha_v = 1.0f;

    f32 coeff = (1.0f / 4.0f) * alpha_u * alpha_v;

    f32 cos_sum = 0.0f;
    for (size_t row = 0; row < HEIGHT; ++row) {
        for (size_t col = 0; col < WIDTH; ++col) {
            f32 pixel_value = block[AT(row, col)];
            
            // @Note: All of those can be pre-computed and encoded in a table/matrix
            // since they are being re-used and are always the same.
            f32 cos_1 = cosf(((2.0f*col + 1) * u*PI) / 16.0f);
            f32 cos_2 = cosf(((2.0f*row + 1) * v*PI) / 16.0f);
            
            cos_sum += pixel_value * cos_1 * cos_2;
        }
    }

    return(coeff * cos_sum);
}

void take_2d_dct(f32 *normalized_block, f32 *dct_block)
{
    for (size_t row = 0; row < HEIGHT; ++row) {
        for (size_t col = 0; col < WIDTH; ++col) {
            dct_block[AT(row, col)] = compute_single_uv(col, row, normalized_block);
        }
    }
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    f32 block[WIDTH * HEIGHT] = {
        52, 55, 61, 66,  70,  61,  64, 73,
        63, 59, 55, 90,  109, 85,  69, 72, 
        62, 59, 68, 113, 144, 104, 66, 73,
        63, 58, 71, 122, 154, 106, 70, 69,
        67, 61, 68, 104, 126, 88,  68, 70,
        79, 65, 60, 70,  77,  68,  58, 75,
        85, 71, 64, 59,  55,  61,  65, 83,
        87, 79, 69, 68,  65,  76,  78, 94,
    };

    printf("\nORIGINAL------------------------------------------------\n\n");    
    print_block(block);
    
    f32 op_block[WIDTH * HEIGHT] = {0};
    center_around_zero(block, op_block);    
    printf("\nNORM----------------------------------------------------\n\n");
    print_block(op_block);
    
    f32 dct_block[WIDTH * HEIGHT] = {0};
    take_2d_dct(op_block, dct_block);    
    printf("\nDCT-II--------------------------------------------------\n\n");
    print_block(dct_block);
    
    return 0;
}
