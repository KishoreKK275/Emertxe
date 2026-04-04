#include "apc.h"
void addition(node *tail1, node *tail2, node **headR, node **tailR)
{
	int sum=0,carry=0; // sum stores digit addition, carry stores carry value
	while (tail1 != NULL || tail2 != NULL) // continue until both lists are fully processed
    {
		int dig1 = 0, dig2 = 0; // store digits from both operands
		if (tail1 != NULL)
        {
            dig1 = tail1->data; // get digit from operand1
            tail1 = tail1->prev; // move to previous digit
        }
		if (tail2 != NULL)
        {
            dig2 = tail2->data; // get digit from operand2
            tail2 = tail2->prev; // move to previous digit
        }
        sum = dig1 + dig2 + carry; // calculate sum including carry
		carry = sum/10; // update carry
        insert_first(headR, tailR, (sum%10)); // insert result digit at beginning
    }
	if(carry) insert_first(headR, tailR, carry); // insert final carry if exists
}
