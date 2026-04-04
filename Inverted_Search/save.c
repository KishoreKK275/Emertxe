#include "inverted.h"
//Function to save database to file
void save_database(M_node *HT[])
{
    // FILE *fp = fopen("database.txt","w");
    char filename[32];// Buffer to hold the filename input by the user
    printf("Enter the file name to save database: ");
    scanf("%s", filename);
    getchar();// Consume the newline character left by scanf
    FILE *fp = fopen(filename, "w");// Open the specified file for writing
    if(!fp)// Check if the file was opened successfully
    {
        printf("Error: Unable to open file database.txt\n");
        return;// Exit the function if the file cannot be opened
    }
    for (int i = 0; i < 27; i++)// Iterate through the hash table
    {
        M_node *mtemp = HT[i];
        while (mtemp)// Traverse the linked list of main nodes at this index
        {
            // Write the main node's data to the file in the specified format
            fprintf(fp, "#%d;%s;%d;", i, mtemp->word, mtemp->file_count);
            S_node *stemp = mtemp->sub_link;// Traverse the linked list of sub nodes for this main node
            while (stemp)// Write each sub node's data to the file
            {
                fprintf(fp, "%s;%d;", stemp->filename, stemp->word_count);
                stemp = stemp->sub_link;// Move to the next sub node
            }
            fprintf(fp, "#\n");// Write the record terminator and a newline after all sub nodes for this main node
            mtemp = mtemp->main_link;// Move to the next main node in the linked list
        }
    }
    fclose(fp);// Close the file after writing all data
    printf("Data Base Saved Successfully!\n");
}
