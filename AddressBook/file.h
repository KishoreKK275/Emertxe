#ifndef FILE_H
#define FILE_H
// include necessary headers for file operations
#include "contact.h"
void saveContactsToFile(AddressBook *addressBook);// Function to save contacts to a file
void loadContactsFromFile(AddressBook *addressBook);// Function to load contacts from a file
void sortContacts(AddressBook *addressBook);
#endif
