#include "apc.h"

void modulus(node *headL1, node *headop2, node *tailop2,node **headR, node **tailR)
{
    if (!headL1 || !headop2)
        return; // return if any operand list is empty
    if (headop2->data == 0 && headop2->next == NULL)
    {
        printf("Error: Modulus by zero\n"); // check for modulus by zero
        insert_last(headR, tailR, 0); //return 0 as result
        return;
    }
    node *temp = headL1;  // pointer to traverse dividend
    if (compare_list(headL1, headop2) == OPERAND2) // if dividend < divisor, remainder = dividend
    {
        node *t = headL1; // copy dividend into result list to avoid double free
        while(t)
        {
            insert_last(headR, tailR, t->data);
            t = t->next;
        }
        return;
    }
    node *headop1 = NULL, *tailop1 = NULL; // temporary list for partial dividend
    while (temp)
    {
        insert_last(&headop1, &tailop1, temp->data);// append next digit to partial dividend
        remove_pre_zeros(&headop1);
        int res = compare_list(headop1, headop2);// compare partial dividend with divisor
        while (res == OPERAND1 || res == SAME) // subtract divisor until smaller
        {
            if(headop1->data < headop2->data) // quick check to avoid unnecessary subtraction
                break;
            node *hTemp = NULL, *tTemp = NULL; // temporary result list
            subtraction(tailop1, tailop2, &hTemp, &tTemp);
            delete_list(&headop1, &tailop1); // delete old partial dividend
            headop1 = hTemp; // update with subtraction result
            tailop1 = tTemp;
            remove_pre_zeros(&headop1);
            if (headop1 == NULL) // if subtraction result becomes empty
            {
                insert_last(&headop1, &tailop1, 0); // insert 0
                break;
            }
            res = compare_list(headop1, headop2); // compare again
        }
        temp = temp->next;
    }
    if (headop1 == NULL)
    {
        insert_last(headR, tailR, 0); // if remainder is empty, result = 0
    }
    else
    {
        *headR = headop1; *tailR = tailop1; // assign remainder list to result
        headop1 = NULL; tailop1 = NULL; // detach temporary pointers
    }
    remove_pre_zeros(headR);
    delete_list(&headop1, &tailop1); // free temporary list if exists
}