#ifndef COMMON_H
#define COMMON_H
/* Magic string to identify whether stegoed or not */
#define MAGIC_STRING "#*"
#define MAX_SECRET_BUF_SIZE 1
//Image buffer size
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
//Extension file size including null
#define MAX_FILE_SUFFIX 4+1
#endif