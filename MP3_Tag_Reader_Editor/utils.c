#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
//Function to print error message
void display_error(const char *message) {
    printf("---------------------ERROR MESSAGE---------------------\n");
    fprintf(stderr, "ERROR: %s\n", message);
    if (strcmp(message, "No arguments provided.") == 0 ||
        strcmp(message, "Invalid arguments.") == 0) {
        printf("USAGE Options:\n");
        printf("To help : ./a.out --help\n");
        printf("To view : ./a.out -v <mp3filename>\n");
        printf("To edit : ./a.out -e <options> <new_text> <mp3filename>\n");
    }
    printf("-------------------------------------------------------\n");
}
//Function to print help message
void display_help() {
    printf("-----------------------HELP MENU-----------------------\n");
    printf("1. -v --> To view MP3 File Contents\n");
    printf("USAGE: ./a.out -v <mp3filename>\n");
    printf("2. -e -> To edit MP3 File Contents\n");
    printf("USAGE: ./a.out -e <options> <new_text> <mp3filename>\n");
    printf("OPTIONS: 2.1 -t : To edit song Title\n");
    printf(" \t 2.2 -a : To edit song Artist Name\n");
    printf(" \t 2.3 -A : To edit Album Name\n");
    printf(" \t 2.4 -y : To edit Year of Release\n");
    printf(" \t 2.5 -n : To edit Genre\n");
    printf(" \t 2.6 -c : To edit Comment\n");
}
TagData* create_tag_data() {
    TagData *data = (TagData *)malloc(sizeof(TagData));
    if (data) 
    {
        data->title = NULL;
        data->artist = NULL;
        data->album = NULL;
        data->year = NULL;
        data->comment = NULL;
        data->genre = NULL;
    }
    return data;
}
void free_tag_data(TagData *data) {
    if (data) 
    {
        free(data->title);
        free(data->artist);
        free(data->album);
        free(data->year);
        free(data->comment);
        free(data->genre);
        free(data);
    }
}
