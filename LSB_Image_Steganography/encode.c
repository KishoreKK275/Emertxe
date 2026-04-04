#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"
//Get the BMP file size
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte(18th byte stores size of the bmp image)
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(uint), 1, fptr_image);
    // printf("width = %u\n", width);
    fread(&height, sizeof(uint), 1, fptr_image);
    // printf("height = %u\n", height);
    return width * height * 3;// 1 pixel = 3 bytes (R+G+B)
}
//Get the secret file size
uint get_file_size(FILE *fptr)
{
    //set fptr to file ending to get the size
    if(fseek(fptr,0,SEEK_END) == fail)
        return fail;
    uint size = ftell(fptr);//store size value in size variable and return
    rewind(fptr);
    return size;
}
//Open all(3) required files in respective modes
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    //open bmp image file and store its fptr in fptr_src_image
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    printf("INFO: Opening required files\n");
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return fail;
    }
    else
    {
        printf("INFO: Opened %s\n",encInfo -> src_image_fname);
    }

    // Secret file
    //open secret file(secret.txt) and store its fptr in fptr_secret
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return fail;
    }
    else
    {
        printf("INFO: Opened %s\n",encInfo->secret_fname);
    }

    // Stego Image file
    //open output file (stego image) and store its fptr in fptr_stego_image
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return fail;
    }
    else
    {
        printf("INFO: Opened %s\n",encInfo->stego_image_fname);
    }
    return success;
}
//Validate arguments
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    if(argc<4 || argc>5)    
        return fail;
    if(!(strstr(argv[2],".bmp")))   
        return fail;
    else
    {
        encInfo -> src_image_fname = argv[2];
        // printf("src_img_fname stored\n");
    }
    char *extn = strchr(argv[3],'.');//.txt
    if(extn==NULL)
        return fail;
    else
    {
        //store secret.txt in secret_fname
        encInfo -> secret_fname = argv[3];
        // printf("secret file name stored\n");
        //store .txt in extn_secret_file
        strncpy(encInfo->extn_secret_file, extn, MAX_FILE_SUFFIX - 1);
        encInfo->extn_secret_file[MAX_FILE_SUFFIX - 1] = '\0';
        // printf("secret file extension name stored\n");
        // encInfo -> extn_secret_file = strchr(argv[3],'.');
    }
    if (argc == 4)
    {
        printf("INFO: Output file not mentioed. Creating steged_img.bmp as default\n");
        //store output file name in stego_imag_fname if not given create default file and store
        encInfo->stego_image_fname = "steged_img.bmp";
        // printf("Default Output file name created\n");
    }
    else
    {
        if (strstr(argv[4],".bmp") != NULL)
            //store output file name in stego_imag_fname if given
            encInfo->stego_image_fname = argv[4];
        else
            return fail;
    }
    return success;
}
//Check capacity of image whether it can handle secret file or not
Status check_capacity(EncodeInfo *encInfo)
{
    //store image size in image_capacity variable (1024 * 768 - sizeof(beautiful.bmp))
   encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
   //store secret file size in size_secret_file variable (24 bytes - sizeof(secret.txt))
   encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
   printf("INFO: Done\n");
   printf("INFO: Checking for %s capacity to handle secret.txt\n",encInfo->stego_image_fname);
   // image header(54)) + Magic string size[#*](2) + secret file extension size[.txt](4) +secret file extension size[.txt size = 4 bytes] + secret file size[int 4 bytes] + secret file data size(int)
   //  54(header) + 2(#*) + 4(extn size) + 4(extn) + 4(file size) + 16 (file) = 84 , image size should be greater than 84*8 = 672 bytes (image size = 23,85,621 bytes)
   if((54 + strlen(MAGIC_STRING) + sizeof(int) + strlen(encInfo-> extn_secret_file) + sizeof(int) + encInfo->size_secret_file)*8 > encInfo->image_capacity)
   {
        printf("ERROR: Secret file is too large to encode in this image\n");
        return fail;
   }
   printf("INFO: Done. Found OK\n");
   return success;
}
//Main Encoding logic file for whole process
Status do_encoding(EncodeInfo *encInfo)
{
    //open all(3) required files
    if(open_files(encInfo) != success)
    {
        printf("ERROR: Opening files failed\n");
        return fail;
    }
    printf("INFO: ## Encoding Procedure Started ##\n");
    printf("INFO: Checking for %s size\n",encInfo->secret_fname);
    if(check_capacity(encInfo) != success)
    {
        printf("ERROR: Secret file size is too large for the given image\n");
        return fail;
    }
    //copy header data safely without touching it
    copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    //encode(store each bit of magic string into 1 byte of stego image after 54 bytes)[#* = 2 bytes * 8 = 16 bytes of magic string into stego image from 55th byte]
    encode_magic_string(MAGIC_STRING,encInfo);
    printf("INFO: Done\n");
    //calulate size of ".txt" (4 bytes) store in variable and pass to fun to encode it into stego file 
    encInfo->size_secret_file_extn = strlen(encInfo->extn_secret_file);
    //encode (size of secret file into stego file)[int]
    encode_secret_file_size_extn(encInfo->size_secret_file_extn, encInfo);
    printf("INFO: Done\n");
    //encode(store extension name into stego image .txt(4 bytes * 8 = 32 bytes into stego image))
    //encode name of extension(.txt) (4 bytes) into stego file[char]
    encode_secret_file_extn(encInfo->extn_secret_file, encInfo);
    printf("INFO: Done\n");
    //encode size of secret file(int) into stego file[int]
    encode_secret_file_size(encInfo->size_secret_file, encInfo);
    printf("INFO: Done\n");
    //next store secret file data(24 bits into 24 * 8 = 192 bytes of stego image)[char]
    encode_secret_file_data(encInfo);
    printf("INFO: Done\n");
    //next store remaining data safely without modifying [char]
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("INFO: Done\n");
    //Close all files before exiting the program
    fclose(encInfo-> fptr_src_image);
    fclose(encInfo-> fptr_stego_image);
    fclose( encInfo-> fptr_secret);
    printf("INFO: ## Encoding completed successfully ##\n");
    return success;
}
//dont change the header data
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //create 54 bytes of buffer(temporary storage) to use as a medium
    char buffer[54];
    printf("INFO: Copying Image Header\n");
    rewind(fptr_src_image);//set bmp image fptr to 0 to start copying data. it is at file ending. you put it there to find its size
    //read 54 bytes and store in buffer and then store this 54 bytes into output stego bmp file without touching it
    if((fread(buffer,sizeof(buffer),1,fptr_src_image)==0) || (fwrite(buffer, sizeof(buffer), 1, fptr_dest_image) == 0))
    {
        printf("INFO: Error Copying image header\n");
        return fail;
    }
    
}
//store magic string first (it is like a password while decoding) 
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Magic String Signature\n");
    //store #* (2 bytes *8 = 16 bytes ) into stego bmp bit by bit
   if( encode_data_to_image(magic_string, strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image)==fail)
    {
        printf("INFO: Error while Encoding magic string ");
        return fail;
    }
    return success;
}
//enocde char data into image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //create 8 bit buffer to get each bit of char 
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
        //read each bit of data from source
        if (fread(image_buffer, 8, 1, fptr_src_image) != 1)
            return fail;
        //store into buffer
        if (encode_byte_to_lsb(data[i], image_buffer) == fail)
            return fail;
        // write it into stego image from buffer 
        if (fwrite(image_buffer, 8, 1, fptr_stego_image) != 1)
           return fail;
    }
    return success;
}
//encode int data(size) into image
Status encode_int_to_image(int data, EncodeInfo *encInfo)
{
    char image_buffer[32];
    // Read 32 bytes from source image
    if (fread(image_buffer, 32, 1, encInfo-> fptr_src_image) == 0)
        return fail;
    // Encode 32 bits of integer into 32 image bytes
    for (int i = 0; i < 32; i++)
    {
        int bit = (data >> (31 - i)) & 1;
        image_buffer[i] = (image_buffer[i] & ~1) | bit;
    }
    // Write modified bytes to stego image
    if (fwrite(image_buffer, 32, 1, encInfo-> fptr_stego_image) == 0)
        return fail;
    return success;
}
//main logic 
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        int x = (data>>(7-i))&1;
        image_buffer[i] = x | (image_buffer[i] &~1);
    }
    return success;
}
//send 1st time extn size(int)
Status encode_secret_file_size_extn(long extn_file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret file extension size\n");//
    if (encode_int_to_image(extn_file_size, encInfo) == fail)
        return fail;
    return success;
}
//send 2nd time extn(char)
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    //home work logic to store extension name second and then data.(first is magic string)
    printf("INFO: Encoding secret file extension\n");//
    return encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
}
//send 3rd time file size(int)
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret file size\n");//
    if (encode_int_to_image(file_size, encInfo) == fail)
        return fail;
    return success;
}
//send 4th type file data(char)
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret file data\n");//
    rewind(encInfo->fptr_secret);
    char ch;
    while (fread(&ch, 1, 1, encInfo->fptr_secret) == 1)
        if (encode_data_to_image(&ch, 1,encInfo->fptr_src_image,encInfo->fptr_stego_image) != success)
            return fail;
    return success;
}
//send last time finally
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_stego)
{
    printf("INFO: Copying remaining image data\n");
    char ch;
    while (fread(&ch, 1, 1, fptr_src) == 1)
        fwrite(&ch, 1, 1, fptr_stego);
    return success;
}