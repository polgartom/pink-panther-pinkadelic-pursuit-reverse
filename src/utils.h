#ifndef _H_UTILS
#define _H_UTILS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <stdint.h>
#include <stdbool.h>

uint32_t decode_16bit_rgb(uint16_t rgb) 
{
    // @Todo: Not 100% right

    uint8_t blue  = (rgb >> 10) & 0b11111;
    uint8_t green = (rgb >> 4) & 0b111111;
    uint8_t red   = rgb & 0b11111;

    blue  = round(blue / 31 * 255);
    green = round(green / 63 * 255);
    red   = round(red / 31 * 255);

    return (red << 16) | (green << 8) | (blue << 0); 
}

#endif