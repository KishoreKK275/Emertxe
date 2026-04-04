#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"
//Validate arguments
Status read_and_validate_decode_args(int argc, char *argv[],  decodeInfo *decInfo)
{
    if(argc<3 || argc>4)
    {
        printf("Decoding: ./a.out -d <bmp_file> [output file]\n");
        return fail;
    }
    if (strstr(argv[2], ".bmp") == NULL)
    {
        printf("ERROR: Stego image must be a .bmp file\n");
        return fail;
    }
    else
        decInfo->stego_image_fname = argv[2];
    if (argv[3] != NULL)
        decInfo->secret_fname = argv[3];
    else
        decInfo->secret_fname = NULL;
    return success;
}
//Skip the header
Status skip_bmp_header(FILE *fptr_stego_image)
{
    //Skip fptr directly to 54 as we dont need header data for secret.txt 
    fseek(fptr_stego_image, 54, SEEK_SET);
    return success;
}
//Open Output 
Status open_output_file(decodeInfo *decInfo)
{
    if (decInfo->secret_fname == NULL)
    {
        printf("INFO: Output file name not provided. Creating decoded.txt as default\n");
        char default_name[50] = "decoded";
        strcat(default_name, decInfo->extn_secret_file);
        decInfo->fptr_secret = fopen(default_name, "wb");
    }
    else
    {
        decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");
        printf("INFO: Creating Secret file with %s. name\n",decInfo->secret_fname);
    }
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        return fail;
    }
    return success;
}
//main logic
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;
    for (int i = 0; i < 8; i++)
        *data = (*data << 1) | (image_buffer[i] & 1);
    return success;
}
//Main Encoding logic file for whole process
Status do_decoding(decodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started ##\n");
    printf("INFO: Opening %s file\n", decInfo->stego_image_fname);//
    //open steged image file for decoding data 
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        printf("ERROR: Error while opening %s file\n",decInfo->stego_image_fname);
        perror("fopen");
        return fail;
    }
    printf("INFO: File opened successfully\n");
    //Skip the 54 bytes header
    skip_bmp_header(decInfo->fptr_stego_image);
    //1. Decode & validate magic string
    if (decode_magic_string(MAGIC_STRING, decInfo) != success)
    {
        fclose(decInfo->fptr_stego_image);//If no magic string close image file and exit
        return fail;
    }
    //2. Decode extension size(4 bytes)(int)
    decode_secret_file_extn_size(decInfo);
    //3. Decode extension name(.txt or .pdf,etc)(char)
    decode_secret_file_extn(decInfo);
    //4. Create output file with decoded extension & file name if given, else default name. create if magic string is matched
    if (open_output_file(decInfo) != success)
        return fail;
    //5. Decode secret file size (int)
    decode_secret_file_size(decInfo);
    //6. Decode secret file data (char)
    decode_secret_file_data(decInfo);
    //7. Close both files & exit 
    fclose(decInfo->fptr_secret);
    fclose(decInfo->fptr_stego_image);
    printf("INFO: ## Decoding completed successfully ##\n");
    return success;
}
//logic to decode char data from image (.txt or file data)(char)
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, 8, 1, fptr_stego_image);
        decode_byte_from_lsb(&data[i], image_buffer);
    }
    return success;
}
//logic to decode int data from image ( .txt size and secret file size)(int)
Status decode_size_from_image(int *data,FILE *fptr_stego_image)
{
    char image_buffer[32];
    *data = 0;
    fread(image_buffer, 32, 1, fptr_stego_image);
    for (int i = 0; i < 32; i++)
        *data = (*data << 1) | (image_buffer[i] & 1);
    return success;
}
//decode & check magic string
Status decode_magic_string(const char *magic_string, decodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String Signature\n");
    int len = strlen(magic_string);//length of our magic string
    //create a temporary char array to read magic string from image and checking with ours 
    char decoded[len + 1];//arr[5]
    decode_data_from_image(decoded, len, decInfo->fptr_stego_image);
    decoded[len] = '\0';//arr[6]
    if (strcmp(decoded, magic_string) != 0)
    {
        printf("ERROR: Magic string mismatch\n");
        return fail;
    }
    printf("INFO: Magic string matched\n");
    return success;
}
//decode secret file extn size (int) 1st time
Status decode_secret_file_extn_size(decodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File Extension Size\n");
    int size;
    if(decode_size_from_image(&size,decInfo->fptr_stego_image) != success)
    {
        printf("ERROR: Secret file extn size not decoded correctly\n");
        return fail;
    }
    decInfo->size_secret_file_extn = size; //store this extn size in variable(.txt= 4 bytes)
    printf("INFO: Done\n");
    return success;
}
//decode secret file extn (char) 2nd time
Status decode_secret_file_extn(decodeInfo *decInfo)
{
    printf("INFO: Decoding Secret file Extension\n");
    if(decode_data_from_image(decInfo->extn_secret_file,decInfo->size_secret_file_extn,decInfo->fptr_stego_image) !=success)
    {
        printf("ERROR: Secret file extension name not decoded correctly\n");
        return fail;
    }
    decInfo->extn_secret_file[decInfo->size_secret_file_extn] = '\0';
    printf("INFO: Done\n");
    return success;
}
//decode secret file size(int) 3rd time
Status decode_secret_file_size(decodeInfo *decInfo)
{
    printf("INFO: Decoding Secret file size\n");
    int size;
    if(decode_size_from_image(&size,decInfo->fptr_stego_image) != success)
    {
        printf("ERROR: Secret file size not decoded correctly\n");
        return fail;
    }
    decInfo->size_secret_file = size;
    printf("INFO: Done\n");
    return success;
}
//decode secret file data(char) last time
Status decode_secret_file_data(decodeInfo *decInfo)
{
    printf("INFO: Decoding Secret file data\n");
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        if(decode_data_from_image(&ch, 1, decInfo->fptr_stego_image) != success)
        {
            printf("ERROR: Secret file data not decoded correctly\n");
            return fail;
        }
        fwrite(&ch, 1, 1, decInfo->fptr_secret);
    }
    printf("INFO: Done\n");
    return success;
}
