#include "inverted.h"
//Function to update database from file
void search_database(M_node *HT[])
{
    char key[32];//Buffer to store the search word
    printf("Enter the word to search : ");
    scanf("%s",key);//Get the search word from user input
    getchar();//Get the index for the word
    int index = get_index(key);//Search for the main node in the hash table using the index
    M_node *mtemp = HT[index];//Traverse the main node list at the index to find the matching word
    while(mtemp)//
    {
        if(strcmp(mtemp->word,key)==0)//If the word is found, print the file count and the list of files with their word counts
        {
            printf("Word found in %d files\n",mtemp->file_count);//Print the file count for the word if the word matches
            S_node *stemp = mtemp->sub_link;
            while(stemp)//Traverse the sub node list to print the filename and word count for each file
            {
                printf("%s : %d\n",stemp->filename,stemp->word_count);//Print the filename and word count for the current sub node if the word matches
                stemp = stemp->sub_link;//Move to the next sub node in the list if the filename does not match
            }
            return;
        }
        mtemp = mtemp->main_link;//Move to the next main node in the list if the word does not match
    }
    printf("Word not found\n");
}
