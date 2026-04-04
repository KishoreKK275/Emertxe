#ifndef APC_H
#define APC_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS		0
#define FAILURE		1

#define SAME		0
#define OPERAND1	1
#define OPERAND2	2
// structure definition for doubly linked list node
typedef struct node
{
    struct node *prev;
    int data;
    struct node *next;
}node;
 // Function prototypes for Operations
void addition(node *tail1, node *tail2, node **headR, node **tailR);
void subtraction(node *tail1, node *tail2, node **headR, node **tailR);
void multiplication(node *tail1, node *tail2, node **headR, node **tailR);
void division(node *headL1, node *headop2, node *tailop2, node **headR, node **tailR);
void modulus(node *headL1, node *headop2, node *tailop2,node **headR, node **tailR);
// Function prototype for input validation
int cla_validation(int argc, char *argv[]);
// Function prototypes for linked list operations
void create_list(char *opr, node **head, node **tail);
int insert_first(node **head, node **tail, int data);
int insert_last(node **head, node **tail, int data);
int delete_list(node **head, node **tail);
void print_list(node *head);
// Function prototypes for utility operations
int compare_list(node *head1, node *head2);
int list_len(node *head);
void remove_pre_zeros(node **head);

#endif