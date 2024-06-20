#include "utils.h"

#define BUFSIZE 128

#define BUF8(_index)  ( ( (uint8_t *)&buf  )[_index] )
#define BUF16(_index) ( ( (uint16_t *)&buf )[_index] )

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("!!!Provide a TEX file!\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror(filename);
        assert(fp != NULL);
    }

    char buf[BUFSIZE] = {0};
    size_t r = 0;

    // 12byte header
    r += fread(buf, 1, 12, fp);
    assert(r == 12);

    bool palette_mode = BUF16(0) == 1;
    uint32_t palette_start_offset = (BUF16(5) << 16) | BUF16(4);
    uint16_t w = BUF16(1);
    uint16_t h = BUF16(2);

    printf(
        "p: %d ; pe: %d ; w: %d ; h: %d\n", 
        palette_mode, 
        palette_start_offset,
        w, 
        h
    );

    if (palette_mode) {

        // while (r != palette_start_offset) {
        //     // @Speed
        //     size_t _r = fread(buf, 1, 1, fp);
        //     assert(_r != 0);
        //     r += _r;

        //     uint16_t index = BUF8(0);
        //     uint32_t rgb = decode_16bit_rgb(index);
        // }

    } else {

    }

    fclose(fp);

    return 0;
}