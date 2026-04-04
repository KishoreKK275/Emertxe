#include "inverted.h"
// Display the database in a tabular format
void display_database(M_node *HT[])
{
    // Print header
    printf("-----------------------------------------------------------------\n");
    printf("| Index | Word            | File count | Filename   | Word count |\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < 27; i++)// Loop through each index in the hash table
    {
        M_node *mtemp = HT[i];// Get the main node at this index
        while (mtemp)// Loop through the main nodes at this index
        {
            S_node *stemp = mtemp->sub_link;// Get the first sub node for this main node
            if (stemp)// If there is at least one sub node, print the main node info with the first sub node
            {
                // Print the main node info along with the first sub node info
                printf("| %-5d | %-15s | %-10d | %-10s | %-10d |\n",i,mtemp->word,mtemp->file_count,stemp->filename,stemp->word_count);
                stemp = stemp->sub_link;// Move to the next sub node
            }
            while (stemp)// If there are more sub nodes, print them without the main node info
            {
                // Print only the sub node info, leaving the main node columns empty
                printf("| %-5s | %-15s | %-10s | %-10s | %-10d |\n","","","",stemp->filename,stemp->word_count);
                stemp = stemp->sub_link;// Move to the next sub node
            }
            printf("-----------------------------------------------------------------\n");
            mtemp = mtemp->main_link;// Move to the next main node at this index
        }
    }
    printf("Displayed Successfully!\n");
}