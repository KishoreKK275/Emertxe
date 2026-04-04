#ifndef READER_H
#define READER_H
#include "utils.h"
//Main Reader Function 
TagData* read_id3_tags(const char *filename);
// Function to display metadata of file
void display_metadata(const TagData *data);
// Function to view tags
void view_tags(const char *filename);
#endif