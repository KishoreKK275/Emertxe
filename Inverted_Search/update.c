#include "inverted.h"
#include <ctype.h>
#define HT_SIZE 27  // 26 for a-z and 1 for non-alphabetic
//Function to get index for hash table
int update_database(M_node *HT[], F_node **head)
{
    char name[32];// buffer for database filename input
    // Get and validate the database filename
    printf("Enter file name: ");
    scanf("%s", name);
    getchar();// consume newline after filename input
    // Validate file extension
    char *ext = strrchr(name, '.');
    if (!ext) {
        printf("INFO : %s -> No extension\n", name);
        return FAILURE;
    }
    // Check if it's a .txt file and not something like .txt.bak
    if (strcmp(ext, ".txt") != 0 || strstr(name, ".txt.") != NULL) {
        printf("INFO : %s -> Not a .txt file\n", name);
        return FAILURE;
    }
    // Check if file exists and is not empty
    FILE *fp = fopen(name, "r");
    if (!fp) {
        printf("INFO : File not available\n");
        return FAILURE;
    }
    // Check if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        printf("INFO : File is empty\n");
        fclose(fp);
        return FAILURE;
    }
    rewind(fp);// reset file pointer to beginning after checks
    // peek – first byte must be '#' to be a valid DB file 
    if (fgetc(fp) != '#') {
        printf("Invalid DB file\n");
        fclose(fp);
        return FAILURE;
    }
    rewind(fp);// reset file pointer to beginning for actual reading

    // buffers for reading each record's components
    char idx_s[5], word[50], fcnt_s[5];
    char fname[50], wcnt_s[5];
    // read each record until EOF
    while (fscanf(fp, " #%[^;];%[^;];%[^;];", idx_s, word, fcnt_s) == 3)
    {
        int idx = atoi(idx_s);
        int fcount = atoi(fcnt_s);
        // SEARCH ONLY IN THAT INDEX
        M_node *mtemp = HT[idx];
        M_node *prev = NULL;
        while (mtemp)// traverse M_node list at this index to find matching word
        {
            if (strcmp(mtemp->word, word) == 0)// found the word
                break;
            prev = mtemp;
            mtemp = mtemp->main_link;
        }
        // CREATE MAIN NODE IF NOT FOUND
        if (!mtemp)
        {
            mtemp = malloc(sizeof(M_node));
            strcpy(mtemp->word, word);
            mtemp->file_count = 0;
            mtemp->sub_link = NULL;
            // insert at head of M_node list for this index
            mtemp->main_link = HT[idx];
            HT[idx] = mtemp;
        }
        // PROCESS SUB NODES
        for (int i = 0; i < fcount; i++)
        {
            // read each <fname>;<wc> pair for this word
            if (fscanf(fp, "%[^;];%[^;];", fname, wcnt_s) != 2)
                break;
            // search sub_link list for this filename
            S_node *stemp = mtemp->sub_link;
            // if not found, create new S_node and link it in; if found, update word_count
            while (stemp)// traverse sub_link list to find matching filename
            {
                // found the filename for this word
                if (strcmp(stemp->filename, fname) == 0)
                    break;
                // move to next S_node
                stemp = stemp->sub_link;
            }
            // if filename not found, create new S_node and link it in; if found, update word_count
            if (!stemp)
            {
                S_node *new_s = malloc(sizeof(S_node));
                strcpy(new_s->filename, fname);
                new_s->word_count = atoi(wcnt_s);
                // link new S_node into sub_link list
                new_s->sub_link = mtemp->sub_link;
                mtemp->sub_link = new_s;
                // increment file_count on M_node since we added a new file for this word
                mtemp->file_count++;
            }
            else
            {
                // filename found, just update the word_count
                stemp->word_count = atoi(wcnt_s);
            }
        }

        fscanf(fp, " #");
    }
    fclose(fp);

    // iterate over each F_node in the linked list
    for (F_node *fptr = *head; fptr != NULL; fptr = fptr->link)
    {
        // scan every bucket in the hash table
        for (int i = 0; i < HT_SIZE; i++)
        {
            M_node *mcur  = HT[i];
            M_node *mprev = NULL;
            // traverse the M_node list in this bucket
            while (mcur)
            {
                // search this word's file list for the CLA filename to remove
                S_node *scur  = mcur->sub_link;
                S_node *sprev = NULL;

                while (scur)// traverse sub_link to find matching filename
                {
                    if (strcmp(scur->filename, fptr->f_name) == 0)
                    {
                        // unlink and free this S_node
                        if (sprev)  sprev->sub_link   = scur->sub_link;
                        else        mcur->sub_link     = scur->sub_link;
                        // free the S_node and decrement file_count on M_node
                        S_node *tmp = scur;
                        scur = scur->sub_link;
                        free(tmp);
                        mcur->file_count--;
                        break;      // a filename can appear at most once per word, so we can stop searching this M_node's sub_link list 
                    }
                    sprev = scur;
                    scur  = scur->sub_link;
                }

                // if no files remain for this word, drop the M_node 
                if (mcur->file_count == 0)
                {
                    if (mprev)  mprev->main_link = mcur->main_link;
                    else        HT[i]            = mcur->main_link;
                    // free the M_node
                    M_node *tmp = mcur;
                    mcur = mcur->main_link;
                    free(tmp);
                }
                else
                {
                    // move to next M_node in this bucket
                    mprev = mcur;
                    mcur  = mcur->main_link;
                }
            }
        }
    }
    printf("Database updated successfully\n");
    return SUCCESS;
}
