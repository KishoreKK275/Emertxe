/*
 Name        : Kishore
 Date 	     : 07-03-2026
 Title       : Arbitrary Precision Calculator
 Description : Driver function for all operations of the APC project.
 *       It performs:
 *               1. Command line validation
 *               2. Sign detection
 *               3. Linked list creation for operands
 *               4. Arithmetic operation execution
 */
#include "apc.h"
int pf1 = 0, pf2 = 0, nf1 = 0, nf2 = 0;
int same_sign(void)
{
    return (pf1 && pf2) || (nf1 && nf2); // returns true if both operands have same sign
}
int main(int argc, char *argv[])
{
    node *head1 = NULL, *tail1 = NULL; // linked list for operand1
    node *head2 = NULL, *tail2 = NULL; // linked list for operand2
    node *headR = NULL, *tailR = NULL; // linked list for result
    if (cla_validation(argc, argv)) //Command line validation
	return 0;
    if (argv[1][0] == '-') nf1 = 1; else pf1 = 1; // Detect signs 
    if (argv[3][0] == '-') nf2 = 1; else pf2 = 1;
    // Create lists
    create_list(argv[1], &head1, &tail1);
    create_list(argv[3], &head2, &tail2);
	// remove leading zeros
    remove_pre_zeros(&head1);
    remove_pre_zeros(&head2);
	//Print Operands
    printf("Num1: "); 
	if(nf1) printf("-");
	else printf("+");
	print_list(head1);
    printf("Num2: "); 
	if(nf2 ) printf("-");
	else printf("+");
	print_list(head2);
    printf("Result: ");
    char oper = argv[2][0];  // store operator
    int cmp = compare_list(head1, head2);  // Compare operands
    switch (oper)
    {
        case '+':
            if (same_sign()) // if both operands have same sign
            {
                if (nf1) printf("-"); // result is -ve if operand1 is -ve
                addition(tail1, tail2, &headR, &tailR);
            }
            else
            {
                if (cmp == OPERAND1) // if operand1 > operand2
                {
                    if (nf1) printf("-");
                    subtraction(tail1, tail2, &headR, &tailR);
                }
                else
                {
                    if (nf2) printf("-");
                    subtraction(tail2, tail1, &headR, &tailR);
                }
            }
            break;
        case '-':
            if (same_sign())// if both operands have same sign
            {
                if (cmp == OPERAND1)
                {
                    if (nf1) printf("-");
                    subtraction(tail1, tail2, &headR, &tailR);
                }
                else
                {
                    if (!nf1) printf("-");
                    subtraction(tail2, tail1, &headR, &tailR);
                }
            }
            else
            {
                if (nf1) printf("-");
                addition(tail1, tail2, &headR, &tailR);
            }
            break;
        case 'x': case 'X':
            multiplication(tail1, tail2, &headR, &tailR);
            if (nf1 ^ nf2) printf("-"); // result -ve if signs are different
            break;
        case '/':
            division(head1, head2, tail2, &headR, &tailR);
            if (nf1 ^ nf2) printf("-"); // result -ve if signs are different
            break;
        case '%':
            modulus(head1, head2, tail2, &headR, &tailR);
            if (nf1) printf("-"); // result -ve if operand1 is -ve
            break;
        default:
            printf("Invalid Input :-( Try again...\n"); // invalid operator
            return FAILURE;
    }
    remove_pre_zeros(&headR); // remove leading zeros from result
	if(headR && headR->data==0) printf("\b"); // formatting adjustment if result is zero
    print_list(headR);
    // printf("Deleting List1:\n"); // free operand1 list
    delete_list(&head1, &tail1);
    // printf("Deleting List2:\n"); // free operand2 list
    delete_list(&head2, &tail2);
	// printf("Deleting Result:\n"); // free result list
    delete_list(&headR, &tailR);
    return SUCCESS;
}