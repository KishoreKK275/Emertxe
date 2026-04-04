#ifndef UTILS_H
#define UTILS_H
// Structure to hold ID3 tag data.
typedef struct 
{
    char *title;   //Title of the song 
    char *artist;  // Artist of the song 
    char *album;   // Album name 
    char *year;    // Year of release
    char *comment; // Comment 
    char *genre;   // Genre 
} TagData;
//Pointer to the newly created TagData structure.
TagData* create_tag_data();
//Frees the memory allocated for the TagData structure.
void free_tag_data(TagData *data);
void display_error(const char *message);
void display_help();
#endif