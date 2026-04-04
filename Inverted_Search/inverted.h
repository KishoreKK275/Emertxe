#ifndef INVERTED_H
#define INVERTED_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define SUCCESS		0
#define FAILURE		1
//Structure definition for file linked list
typedef struct file
{
    char f_name[20];
    struct file *link;
}F_node;
//Structure definition for main node of hash table
typedef struct Sub
{
    int word_count;
    char filename[20];
    struct Sub *sub_link;
}S_node;
//Structure definition for sub node of hash table
typedef struct Main
{
    char word[25];
    int file_count;
    S_node *sub_link;
    struct Main *main_link;
}M_node;

//Function prototypes
void create_database(M_node *HT[], F_node *head);
void display_database(M_node *HT[]);
void search_database(M_node *HT[]);
void save_database(M_node *HT[]);
int update_database(M_node *HT[],F_node **head);
void free_database(M_node *HT[]);
//Function prototypes for file linked list operations
void validate_files(char *argv[], F_node **head);
int insert_last(F_node **head, char *f_name);
int check_duplicates(F_node *head, char *f_name);
int print_filenames(F_node *head);
int get_index(char *word);

#endif
