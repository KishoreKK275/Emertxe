/*
NAME        :   Kishore
DATE        :   15-01-2026
PROJECT     :   LSB Image Steganography
DESCRIPTION :  
        This project implements LSB (Least Significant Bit) steganography for BMP images to securely hide and 
extract secret data. The system embeds information into the least significant bits of pixel values without 
causing noticeable changes to the image. The implementation includes comprehensive input validation, file format 
validation, and robust error handling to ensure data integrity and reliable operation. The project demonstrates a 
practical approach to information hiding while maintaining image quality and system stability.
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "common.h"
#include "types.h"

//Function to check given operation type 
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1],"-e") == 0)
        return encode;
    if (strcmp(argv[1],"-d") == 0)
        return decode;
    return unsupported;
}
//Main function
int main(int argc, char* argv[])
{
    //Structure Pointers which holds the data
    EncodeInfo encInfo;
    decodeInfo decInfo;
    if (argc < 2)
    {
        printf("Encoding: ./a.out -e <bmp_file> <.txt file> [output file]\n");
        printf("Decoding: ./a.out -d <bmp_file> [output file]\n");
        return fail;
    }
    if (argc == 3 && (strcmp(argv[1], "-e") == 0))
    {
        printf("Encoding: ./a.out -e <bmp_file> <.txt file> [output file]\n");
        return fail;
    }
    if (argc == 2 && (strcmp(argv[1], "-d") == 0))
    {
        printf("Decoding: ./a.out -d <bmp_file> [output file]\n");
        return fail;
    }
    OperationType res = check_operation_type(argv);
    if(res == encode)
    {
        printf("INFO: Encoding selected\n");
        if(read_and_validate_encode_args(argc,argv,&encInfo)==success)
        {
            // printf("INFO: Argument validation successful\n");
            do_encoding(&encInfo);//Call Encoding Function
        }
    }
    else if(res == decode)
    {
        printf("INFO: Decoding selected\n");
        if(read_and_validate_decode_args(argc,argv,&decInfo)==success)
        {
            // printf("INFO: Argument validation successful\n");
            do_decoding(&decInfo);//Call Decoding Function
        }
    }
    else
    {
        printf("ERROR: Argument validation failed\n");
        printf("ERROR: Unsupported operation\n");
        return fail;//Print error message & exit
    }
    return success;
}