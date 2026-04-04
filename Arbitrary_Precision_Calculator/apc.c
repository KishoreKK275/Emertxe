#include "apc.h"
#include <ctype.h>
int cla_validation(int argc, char *argv[])
{
    if(argc != 4) // check correct number of arguments
    {
        printf("USAGE: ./a.out <operand1> <operator> <operand2>\n");
        exit(0);
    }
    if(strlen(argv[2]) != 1 || !strchr("+-xX/%%", argv[2][0]))
    {
        printf("Error: Invalid Operator\n"); // check if operator is valid
        exit(0);
    }
    int i = (argv[1][0] == '-' || argv[1][0] == '+') ? 1 : 0; // skip '-' if operand1 is negative
    if(argv[1][i] == '\0') // check empty operand1
    {
        printf("Error: Invalid Operand 1\n");
        exit(0);
    }
    while (argv[1][i] != '\0')
    {
        if (!isdigit(argv[1][i])) // validate digits of operand1
        {
            printf("Error: Invalid Operand 1\n");
            exit(0);
        } i++;
    }
    i = (argv[3][0] == '-' || argv[3][0] == '+')  ? 1 : 0; // skip '+' if operand1 is negative
    if(argv[3][i] == '\0') // check empty operand2
    {
        printf("Error: Invalid Operand 2\n");
        exit(0);
        // return FAILURE;
    }
    while (argv[3][i] != '\0')
    {
        if (!isdigit(argv[3][i])) // validate digits of operand2
        {
            printf("Error: Invalid Operand2 \n");
            exit(0);
        } i++;
    }
    return 0;
}
int list_len(node *head)
{
    int count = 0;
    while(head!=NULL) // traverse list and count nodes
    {
        count++;
        head = head->next;
    }
    return count; // return list length
}

int compare_list(node *head1, node *head2)
{
    int l1 = list_len(head1), l2 = list_len(head2);// get lengths of both lists
    if(l1>l2) return OPERAND1; // operand1 is larger
    else if(l2>l1) return OPERAND2; // operand2 is larger
    else
    {
        node *t1 = head1, *t2 = head2;
        while(t1!=NULL && t2!=NULL) // traverse both lists
        {
            // compare digits
            if(t1->data > t2->data) return OPERAND1;
            else if(t1->data < t2->data) return OPERAND2;
            else
            {
                t1 = t1->next;
                t2 = t2->next;
            }
        }
        return SAME; // numbers are equal
    }
}
void create_list(char *opr, node **head, node **tail)
{
    int i=0;
    while(opr[i]!='\0') // traverse operand string
    {
        if(!isdigit(opr[i])) // skip non-digit characters (like '-')
        {
            i++;
            continue;
        }
        char c = opr[i];// get current character
        // if(opr[i]==' ') break;
        int x = c - '0';// convert char to int
        if(insert_last(head, tail, x))// insert digit into list
        {
            printf("Error: While creating list\n");// check for insertion failure
            return ;
        }
        i++;
    }
}
void remove_pre_zeros(node **head)
{
    if(*head == NULL) return;// if list is empty, return
    node *temp = *head;
    while(temp->next!=NULL && temp->data==0) // traverse list until first non-zero digit or end of list
    {

        node *t2=temp;// store current node in temp2
        temp = temp->next;// move temp to next node
        free(t2);// free temp2 (leading zero node) 
        temp->prev = NULL;// set new head's prev to NULL
    }
    *head = temp;
}

int insert_first(node **head, node **tail, int data)
{
    node *new_node = malloc(sizeof(node));
    if(new_node==NULL) 
     return FAILURE;
    new_node->data=data;
    new_node->prev=NULL;
    new_node->next=NULL;
    node *temp = *head;
    if(*head==NULL)
    {
        *head = (*tail = new_node);
        return SUCCESS;
    }
    else
    {
        temp->prev=new_node;
        new_node->next=temp;
        *head = new_node;
        return SUCCESS;
    }
}

int insert_last(node **head, node **tail, int data)
{
    node *new_node = malloc(sizeof(node));
    if(new_node==NULL) return FAILURE;
    new_node->data=data;
    new_node->prev=NULL;
    new_node->next=NULL;
    if(*head==NULL)
    {
        *head = (*tail = new_node);
        return SUCCESS;
    }
    else
    {
        (*tail)->next=new_node;
        new_node->prev=*tail;
        *tail = new_node;
        return SUCCESS;
    }
}

void print_list(node *head)
{
	if (head == NULL)
		printf("INFO : List is empty\n");
	else
	{
	    while (head)		
	    {
		    printf("%d", head -> data);
		    head = head -> next;
	    }
    }
    printf("\n");
}

int delete_list(node **head, node **tail)
{
    if(*head==NULL) return FAILURE;
    node *temp = *head;
    if(*head==*tail)
    {
        free(*head);
        *head = *tail = NULL;
        return SUCCESS;
    }
    while (*head != NULL)
    {
        temp = *head;//store struct in temp (12)
        *head = temp->next;//store next link in head 
        free(temp);//free struct
    }
    return SUCCESS;
}