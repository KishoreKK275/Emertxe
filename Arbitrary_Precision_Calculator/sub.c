#include "apc.h"
void subtraction(node *tail1, node *tail2, node **headR, node **tailR)
{
	int res=0,borrow=0; // res stores subtraction result, borrow handles borrowing
	while (tail1 != NULL || tail2 != NULL) // continue until both lists are processed
    {
		int dig1 = 0, dig2 = 0;  // store digits from operands
		if (tail1 != NULL)
        {
            dig1 = tail1->data;  // get digit from operand1
            tail1 = tail1->prev; // move to previous digit
        }
		if (tail2 != NULL)
        {
            dig2 = tail2->data; // get digit from operand2
            tail2 = tail2->prev; // move to previous digit
        }
		dig1 = dig1 + borrow;  // apply previous borrow
		if(dig1<dig2) // check if borrowing is required
		{
			dig1 += 10; // borrow from next digit
			borrow = -1;
		}
		else borrow = 0;
		res = dig1 - dig2 ; // perform subtraction
        insert_first(headR, tailR, res); // insert result digit at beginning
    }
}