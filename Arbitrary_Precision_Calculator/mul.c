#include "apc.h"

void multiplication(node *tail1, node *tail2, node **headR, node **tailR)
{
	int prod=0,carry=0,count=0,k=0; // prod -> product of digits, carry -> carry value, count -> number of shifts
    node *t1, *t2 = tail2; // t1 and t2 used to traverse operand lists from tail
    node *headR2 = NULL, *tailR2 = NULL; // temporary list to store partial product
    node *headAR = NULL, *tailAR = NULL; // list to store addition result
    while (t2)
    {
        carry = 0; t1=tail1; // reset pointer for operand1
        // printf("\n---- OUTER LOOP START ----\n");
        // printf("t2 digit = %d\n", t2->data);
        // printf("count = %d\n", count);
        // printf("Adding %d zeros to R2\n", count);
        for(k = 0; k < count; k++)
        {
            insert_first(&headR2,&tailR2,0); // add zeros based on place value shift
        }
        // printf("R2=");print_list(headR2);
        while(t1) // inner loop for each digit of operand1
        {
            // printf("\nOperation = %d * %d + %d\n", t1->data, t2->data,carry);
            prod = (t1->data)*(t2->data) + carry; // multiply digits and add carry
            // printf("prod = %d\n", prod);
            carry = prod/10; //update carry
            // printf("Inserting %d into R2:\n",prod%10);
            // printf("carry = %d\n", carry);
            insert_first(&headR2,&tailR2, (prod%10)); // insert result digit
            // printf("Current Lists State:\n");
            // printf("R  = "); print_list(*headR);
            // printf("R2 = "); print_list(headR2);
            // printf("AR = "); print_list(headAR);
            t1 = t1->prev; //move to previous digit
        }
        // printf("\n-- Finished inner loop --\n");
        // printf("Final carry after inner loop = %d\n", carry);
        if (carry)
            insert_first(&headR2, &tailR2, carry);//insert remaining carry
        if(count == 0) // first partial product becomes result
        {
            *headR = headR2;
            *tailR = tailR2;
        }
        else
        {
            // printf("Adding R an R2 & storing to AR\n");
            addition(*tailR, tailR2, &headAR, &tailAR); // add previous result and current partial product
            // printf("AR = "); print_list(headAR);
            delete_list(headR,tailR);//delete old result list
            // delete_list(&headR2,&tailR2);
            *headR = headAR; // update result with addition result
            *tailR = tailAR;
        }
        headR2 = tailR2 = NULL; // reset partial product list
        // printf("Updated R = "), print_list(*headR);
        headAR=tailAR=NULL; // reset addition result pointers 
        t2 = t2->prev; // move to next digit of operand2
        count++; // increase shift count
    }
    // printf("\nHandling remaining carry outside loops\n");
    if (carry)
        insert_first(&headR2, &tailR2, carry); // insert carry if remains
    // print_list(*headR);   
}