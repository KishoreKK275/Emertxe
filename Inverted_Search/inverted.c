#include "inverted.h"
// Function to validate files and create a linked list of valid files
void validate_files(char *argv[], F_node **head)
{
    int i=1,j=0;
    while(argv[i])
    {
        // char *ext = strstr(argv[i],".txt");
        char *ext = strrchr(argv[i], '.');
        //Check no extension
        if(ext == NULL)
        {
            printf("INFO : %s \u2192 This file does not have an extension\n",argv[i++]);
            continue;
        }
        //Check invalid extension
        if(strcmp(ext,".txt")!=0 || strstr(argv[i], ".txt.") != NULL)
        {
            printf("INFO : %s \u2192 This is not a .txt file\n",argv[i++]);
            continue;
        }
        //Check file availability
        FILE *fp = fopen(argv[i], "r");
        if(fp == NULL)
        {
            printf("INFO : %s \u2192 File not available \n", argv[i++]);
            continue;
        }
        //Check empty file
        fseek(fp, 0, SEEK_END);
        if(ftell(fp) == 0)
        {
            printf("INFO : %s → This file is empty\n", argv[i++]);
            fclose(fp);
            continue;
        }
        rewind(fp);
        //Check duplicate & Store in SLL
        if(check_duplicates(*head,argv[i]) == FAILURE)
        {
            insert_last(head,argv[i]);
            printf("INFO : Successful: inserting %s into file linked list\n",argv[i++]);
        }
        else
            printf("INFO : %s \u2192 This file is already added\n",argv[i++]);
        fclose(fp);
    }
    print_filenames(*head);
}
//Function to insert a file name at the end of the linked list
int insert_last(F_node **head, char *f_name)
{
    F_node *new_node = (F_node*)malloc(sizeof(F_node));
    if(!new_node)
    {
        printf("INFO : Memory allocation failed\n");
        return FAILURE;
    }
    strcpy(new_node->f_name,f_name);
    new_node->link = NULL;
    if(*head == NULL)
    {
        *head = new_node;
        return SUCCESS;
    }
    F_node *temp = *head;
    while(temp->link)
        temp = temp->link;
    temp->link = new_node;
    return SUCCESS;
}
//Function to check duplicate file names in the linked list
int check_duplicates(F_node *head, char *f_name)
{
    while(head)
    {
        if(strcmp(head->f_name,f_name) == 0)
            return SUCCESS;
        head = head->link;
    }
    return FAILURE;
}
//Function to print the file names in the linked list
int print_filenames(F_node *head)
{
    printf("\nFile list: \n");
    if(head == NULL)
    {
        printf("INFO : No valid files are there\nCreate database is not possible\n");
        return FAILURE;
    }
    while(head)
    {
        printf("%s \u2192 ",head->f_name);
        head = head->link;
    }
    printf("NULL\n\n");
    return SUCCESS;
}