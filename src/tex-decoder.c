#include "utils.h"
#include "libbmp/libbmp.h"


#define EAT_BYTES(_bytes) \
    assert(i + _bytes <= fsize); \
    i += _bytes; \
    dat += _bytes; \

void save_img(bmp_img img, char *fname)
{
    static u32 file_no = 0;
    file_no += 1;

    size_t k = find_char_index_from_right(fname, '/');
    char *output_fname = fname+k+1;
    static char output_path[128] = {0};
    memset(output_path, 0, 128);
    size_t output_path_size = STR_LEN("./output/") + STR_LEN(output_fname) + STR_LEN(".bmp");
    assert(output_path_size+1 < 128);

    int result = snprintf(output_path, 128, "./output/%s_%d.bmp", output_fname, file_no);
    assert(result > 0);

    enum bmp_error r = bmp_img_write(&img, output_path);
    assert(r == BMP_OK);

    printf("|%s| -> |%s| -- ok!\n", fname, output_path);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("!!!Provide a TEX file!\n");
        return 1;
    }

    size_t i = 0;

    char *fname  = argv[1];
    size_t fsize = 0;
    u8 *dat = read_entire_file(fname, &fsize);
    const u8 *dat_original = dat;

    assert(fsize > 12);
    u16 *header = (u16 *)dat;
    EAT_BYTES(12);

    bool palette_mode = header[0] == 1;
    u16 w = header[1];
    u16 h = header[2];
    u32 palette_start_offset = (header[5] << 16) | header[4];
    assert(fsize > palette_start_offset);

    printf(
        "palette_mode: %s ; palette_start_offset: %d ; width: %d ; height: %d\n", 
        palette_mode ? "YES" : "NO", 
        palette_start_offset,
        w, 
        h
    );

    bmp_img img;
	bmp_img_init_df(&img, w, h);

    u32 x = 0;
    u32 y = 0;

    if (palette_mode) {

        u32 stream_end = palette_start_offset - 12;
        u16 *palette = (u16 *)(dat_original + palette_start_offset);
        // u64 rem_size = fsize - (u64)((u8*)palette - (u8*)dat_original);

        while (i != stream_end) {
            u8 color_start_addr = *dat;
            assert( (u64)(palette + color_start_addr + 2) < (u64)(dat_original + fsize) );

            u16 rgb16 = palette[color_start_addr];
            // printf("%d\n", rgb16);

            RGB32 c = decode_16bit_rgb(rgb16);
            bmp_pixel_init(&img.img_pixels[y][x], c.r, c.g, c.b);

            x += 1;
            if (x == w) {
                y += 1;
                x = 0;
            }

            EAT_BYTES(1);
        }

    } else {

        while (i < fsize) {
            u16 rgb16 = *((u16 *)dat);
            RGB32 c = decode_16bit_rgb(rgb16);
            bmp_pixel_init(&img.img_pixels[y][x], c.r, c.g, c.b);

            x += 1;
            if (x == w) {
                y += 1;
                x = 0;

                if (!(y < h)) {
                    break;
                    // record.tex has multiple in it. What is that? Moving picture? Multiple resolution?
                    //     y = 0;
                    //     save_img(img, fname);
                }

            }

            EAT_BYTES(2);
        }

    }

    save_img(img, fname);

	bmp_img_free(&img);
    printf("i: %ld ; fsize: %ld\n", i, fsize);

    return 0;
}