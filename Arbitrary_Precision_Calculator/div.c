#include "apc.h"

void division(node *headL1, node *headop2, node *tailop2,node **headR, node **tailR)
{
	if (!headL1 || !headop2)
        return; // return if any operand list is empty
    if (headop2->data == 0 && headop2->next == NULL)
    {
        printf("Error: Division by zero\n"); // check for division by zero
		exit(0);
    }
    if (compare_list(headL1, headop2) == OPERAND2)
    {
        insert_last(headR, tailR, 0); // if dividend < divisor, quotient = 0
        return;
    }
    node *headop1 = NULL, *tailop1 = NULL; // temporary list to hold partial dividend
	node *temp = headL1; // pointer to traverse dividend
    while (temp)
    {
		insert_last(&headop1, &tailop1, temp->data); // append next digit to partial dividend
		remove_pre_zeros(&headop1); // remove leading zeros
        int count = 0;
		int res = compare_list(headop1, headop2); // compare partial dividend with divisor
		while (res == OPERAND1 || res == SAME) // subtract divisor until smaller
        // while ((compare_list(headop1, headop2) == OPERAND1)||(compare_list(headop1, headop2) == SAME))
        {
			if(headop1->data < headop2->data) // skip unnecessary subtraction
                break;
            node *hTemp = NULL, *tTemp = NULL; // temporary result list
            subtraction(tailop1, tailop2, &hTemp, &tTemp);
            delete_list(&headop1, &tailop1); // delete old partial dividend
            headop1 = hTemp; // update with subtraction result
            tailop1 = tTemp;
            remove_pre_zeros(&headop1);
            count++; // increment quotient digit
			if (headop1 == NULL) // if subtraction result becomes empty
			{
				insert_last(&headop1, &tailop1, 0); //insert 0
				break;
			}
			res = compare_list(headop1, headop2); // compare again for next subtraction
        }
        insert_last(headR, tailR, count); // store quotient digit
        temp = temp->next; //move to next node(nexit digit of dividend)
    }
    remove_pre_zeros(headR);
    delete_list(&headop1, &tailop1); // free temporary list
}