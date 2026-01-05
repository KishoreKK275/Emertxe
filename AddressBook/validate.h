#ifndef VALIDATE_H 
#define VALIDATE_H
// Header file for validation functions
#include "contact.h"
// Function prototypes
int validatename(char name[]);//Function to validate name
int validateno(char phone[]);//Function to validate phone number
int validatmail(char mail[]);//Function to validate email
int checkDuplicateno(char phone[],AddressBook *addressBook);//Function to check duplicate phone number
int checkDuplicatename(char name[],AddressBook *addressBook);//Function to check duplicate name
int checkDuplicatmail(char mail[],AddressBook *addressBook);//Function to check duplicate email

#endif