//include necessary headers
#include <stdio.h>
#include <string.h>
//include other custom headers
#include "file.h"
#include "contact.h"
#include "ui.h"
// Implement file operations for saving, sorting and loading contacts
// Function to save contacts to a file
void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp;  
    if ((fp = fopen("contacts.csv", "w")) == NULL)
    {
        printf(RED"Error: File didn't open correctly.\n"RESET);
        return;   
    }
     fprintf(fp, "#%d\n", addressBook->contactCount);
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fp, "%d.", i+1);                 
        fprintf(fp, "%s,", addressBook->contacts[i].name);     
        fprintf(fp, "%s,", addressBook->contacts[i].phone);    
        fprintf(fp, "%s\n", addressBook->contacts[i].mail);    
    }
    fclose(fp);
}
// Function to load contacts from a file
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp;
    if ((fp = fopen("contacts.csv", "r")) == NULL)
    {
        printf(RED"Error: File didn't open correctly.\n"RESET);
        return;
    }
    if (fscanf(fp, "#%d\n", &addressBook->contactCount) != 1)
    {
        fclose(fp);
        return;
    }
     for (int i = 0; i < addressBook->contactCount; i++)
    {
        int index;
        fscanf(fp,"%d.%[^,],%[^,],%s",&index,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].mail);
    }
    fclose(fp);
}
// Function to sort contacts from a file
void sortContacts(AddressBook *addressBook)
{
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (int j = 0; j < addressBook->contactCount - 1 - i; j++)
        {
            if (strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name) > 0)
            {
                Contact x = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = x;
            }
        }
    }
}