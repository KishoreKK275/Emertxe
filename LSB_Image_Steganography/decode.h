#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include "common.h"
#include "types.h"
/* 
 * Structure to store information required for
 * decoding secret file from Stegnoed Image
 * Info about output and intermediate data is also stored
 */
typedef struct _decodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    int size_secret_file_extn;
    long size_secret_file;

} decodeInfo;

/* decoding function prototypes */
/* Read and validate decode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], decodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(decodeInfo *decInfo);

/* Get File pointers for o/p file */
Status open_output_file(decodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_src_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, decodeInfo *decInfo);

/* decode secret file extenstion size */
Status decode_secret_file_extn_size(decodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(decodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(decodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(decodeInfo *decInfo);

/* decode function, which does the real decoding (for data(char))*/
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);

/* decode function, which does the real decoding (for size(int))*/
Status decode_size_from_image(int *data, FILE *fptr_stego_image);

/* decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

#endif