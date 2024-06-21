#ifndef _H_UTILS
#define _H_UTILS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  float32;
typedef double float64;

#define STR_LEN(_str) (_str != NULL ? strlen(_str) : 0)

////////////////////////////////////////////

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} RGB32;

RGB32 decode_16bit_rgb(u16 rgb) 
{
    // @Todo: Not 100% right
    u8 blue  = (rgb >> 10) & 0b11111;
    u8 green = (rgb >> 4) & 0b111111;
    u8 red   = rgb & 0b11111;

    blue  = round( ((float32)blue ) / 31 * 255);
    green = round( ((float32)green) / 63 * 255);
    red   = round( ((float32)red  ) / 31 * 255);

    // printf("%d ; %d ; %d\n", red, green, blue);
    // return (red << 16) | (green << 8) | (blue << 0);

    RGB32 c = {red, green, blue}; 
    return c;
}

u8 *read_entire_file(char *fname, size_t *fsize)
{
    FILE *fp = fopen(fname, "rb");
    if (fp == NULL) {
        perror(fname);
        assert(fp != NULL);
    }

    fseek(fp, 0, SEEK_END);
    *fsize = ftell(fp);
    rewind(fp);

    u8 *dat = (u8 *)malloc(*fsize);
    memset(dat, 0, *fsize);

    fread(dat, 1, *fsize, fp);
    fclose(fp);

    return dat;
}

size_t find_char_index_from_right(char *path, char subject)
{
    assert(path);
    size_t len = STR_LEN(path); 
    if (len == 0) return -1;

    char *start_addr = path;
    char *end_addr   = path + len - 1; 
    while (end_addr != path) {
        if (*end_addr == subject) {
            return end_addr - start_addr;
        }
        end_addr -= 1;
    }

    return -1;
}

#endif