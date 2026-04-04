#include "inverted.h"
#include <ctype.h>
int get_index(char *word)
{
    int i;
    if(isalpha(word[0]))
    {
        if(islower(word[0])) i = word[0]-'a';
        // if(isupper(word[0]))
        else i = word[0] - 'A';
    }
    else i = 26;
    return i;
}

void create_database(M_node *HT[], F_node *head)
{
    F_node *temp = head;
    while(temp)
    {
        //Open file in read mode(one by one) for each file [open file do operations and close]
        FILE *fp = fopen(temp->f_name,"r");
        if(!fp)
        {
            printf("Error: Unable to open file %s\n", temp->f_name);
            temp = temp->link;
            continue;
            // printf("Error: File not opened\n");
            // return;
        }

        char buf[32];

        // Read a word(oneby word) use fscanf
        while(fscanf(fp,"%30s",buf) != EOF)
        {
            // Call get index and find the index
            int i = get_index(buf);

            // FIX: Search ONLY in that index (not full HT)
            M_node *mtemp = HT[i];
            M_node *prev_m = NULL;

            while(mtemp)
            {
                if(strcmp(mtemp->word, buf) == 0)
                    break;

                prev_m = mtemp;
                mtemp = mtemp->main_link;
            }

            //if main node link is null then create main and subnodes
            // Case 1 : No main node present
            if(mtemp == NULL)
            {
                //Create main node and assign values
                M_node *mnode = malloc(sizeof(M_node));
                if(!mnode)
                {
                    printf("Malloc failed for New Main node\n");
                    fclose(fp);
                    return;
                }

                strcpy(mnode->word,buf);
                mnode->file_count = 1;

                //Create subnode and assign values
                S_node *snode = malloc(sizeof(S_node));
                if(!snode)
                {
                    printf("Malloc failed for New Sub node\n");
                    fclose(fp);
                    return;
                }

                strcpy(snode->filename, temp->f_name);
                snode->word_count = 1;
                snode->sub_link = NULL;

                //Link Main node with Subnode
                mnode->sub_link = snode;

                // FIX: Proper chaining (no overwrite)
                mnode->main_link = HT[i];
                HT[i] = mnode;
            }

            // Case 2 : Main node present
            else
            {
                //Search word in main nodes 
                while(mtemp)
                {
                    // compare 
                    if(strcmp(mtemp->word, buf) == 0)
                        break;

                    prev_m = mtemp;
                    mtemp = mtemp->main_link;
                }

                // Case 2.a : Matching word found
                if(mtemp)
                {   
                    // word found in main node list
                    // search filename in sub node list
                    S_node *stemp = mtemp->sub_link;
                    S_node *prev_s = NULL;

                    while(stemp)
                    {
                        if(strcmp(stemp->filename, temp->f_name) == 0)
                            break;

                        prev_s = stemp;
                        stemp = stemp->sub_link;
                    }

                    // Case 2.a.i : Matching file name found
                    if(stemp)
                    {   
                        // filename found in sub node list
                        // increment word count
                        stemp->word_count++;
                    }

                    // Case 2.a.ii : Matchine file not found, create new
                    else
                    {   
                        // filename not found in sub node list
                        // create new sub node
                        S_node *snode = malloc(sizeof(S_node));
                        if(!snode)
                        {
                            printf("Malloc failed for New Sub node\n");
                            fclose(fp);
                            return;
                        }

                        strcpy(snode->filename, temp->f_name);
                        snode->word_count = 1;
                        snode->sub_link = NULL;

                        //Link Main node with Subnode
                        // prev_s->sub_link = snode;
                        if (prev_s)     
                            prev_s->sub_link = snode;
                        else    
                            mtemp->sub_link = snode;

                        mtemp->file_count++;
                    }   
                }

                // Case 2.b : Matching word not found
                else
                {   
                    // word not found in main node list
                    // create new main node
                    M_node *mnode = malloc(sizeof(M_node));
                    if(!mnode)
                    {
                        printf("Malloc failed for New Main node\n");
                        fclose(fp);
                        return;
                    }

                    strcpy(mnode->word,buf);
                    mnode->file_count = 1;

                    //Create subnode and assign values
                    S_node *snode = malloc(sizeof(S_node));
                    if(!snode)
                    {
                        printf("Malloc failed for New Sub node\n");
                        fclose(fp);
                        return;
                    }

                    strcpy(snode->filename, temp->f_name);
                    snode->word_count = 1;
                    snode->sub_link = NULL;

                    //Link Main node with Subnode
                    mnode->sub_link = snode;

                    // FIX: Proper chaining instead of overwrite
                    mnode->main_link = HT[i];
                    HT[i] = mnode;
                }   

                 // if new word then traverse and create and insert last
                 // else update the old matched node with -->
                 //if same file name and same word update word count in subnode
                 //else create new subnode for that word and store new subnode address in previous subnode, and store filename, word count
            }
        }

        fclose(fp);
        temp = temp->link;
    }

    printf("Data Base Created Successfully!\n");   
}

