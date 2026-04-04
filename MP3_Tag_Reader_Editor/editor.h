#ifndef EDITOR_H
#define EDITOR_H
#include "utils.h"
//Main function to perform Edit operations
TagData *edit_tag(const char *option, const char *new_text, const char *filename);
int edit_tags(const char *option, const char *new_text, const char *filename);
#endif 