#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100// Maximum number of contacts
// Define the Contact structure
typedef struct {
    char name[50];// Full name
    char phone[11];//Phone number
    char mail[50];// Email address
} Contact;// Variable to hold contact information
typedef struct {
    Contact contacts[100];//Array to hold multiple contacts
    int contactCount;// Variable to hold the number of contacts
} AddressBook;// Variable to hold the address book
// Function prototypes
void print();
void createContact(AddressBook *addressBook);// Function to create a new contact
void searchContact(AddressBook *addressBook);// Function to search for a contact
void editContact(AddressBook *addressBook);// Function to edit a contact
void deleteContact(AddressBook *addressBook);// Function to delete a contact
void listContacts(AddressBook *addressBook);// Function to list all contacts
void initialize(AddressBook *addressBook);// Function to initialize the address book
void save(AddressBook *addressBook);// Function to save and exit
void search(AddressBook *addressBook,int);// Function to search for a contact by index
#endif
