#include <stdio.h>
#include <string.h>
#include "reader.h"
#include "editor.h"
#include "utils.h"
int main(int argc, char *argv[]) {
    if (argc <= 1 ) {
        display_error("No arguments provided.");
        return 1;
    }
    else if (strcmp(argv[1], "--help") == 0 && argc ==2) {
        display_help();
        return 0;
    } 
    else if (strcmp(argv[1], "-v") == 0 && argc == 3) {
        char *filename = argv[2]; // Temp buf to store filename
        char *ext = strrchr(filename, '.'); // Checking . is there or not
        if (!ext || strcmp(ext, ".mp3") != 0) { // Checking .mp3 is there or not
            display_error("Invalid file");
            return 1;
        }
        FILE *f = fopen(filename, "rb");
        if (!f) {//Checking for invalid file
            display_error("Invalid file");
            fclose(f); return 1;
        }
        fclose(f);
        view_tags(argv[2]);
    }
    else if (strcmp(argv[1], "-e") == 0 && argc == 5) {
        char *ext = strrchr(argv[4], '.');//Checking . 
        if (!ext || strcmp(ext, ".mp3") != 0 ) { // Checking .mp3 is there or not
            display_error("Invalid file ");
            return 1;
        }
        //Temporary buffers to store option, New Text and File name
        char *option = argv[2];
        char *text = argv[3];
        char *filename = argv[4];
        FILE *f = fopen(filename, "rb");
        if (!f) {//Checking for invalid file
            display_error("Invalid file");
            fclose(f); return 1;
        }
        fclose(f);
        if (edit_tags(option, text, filename)) {
            display_error("Failed to edit tag.");
            return 1;
        }
    }
    else {
        display_error("Invalid arguments.");
        return 1;
    }
    return 0;
}