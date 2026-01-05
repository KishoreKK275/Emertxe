//include custom header files for address book and contact structures
#include "contact.h"
void populateAddressBook(AddressBook* addressBook);// Function to populate the address book with initial data
static Contact dummyContacts[] = {  // Dummy contact data
    {"John Doe", "1234567890", "john@example.com"},
    {"Alice Smith", "0987654321", "alice@example.com"},
    {"Bob Johnson", "1112223333", "bob@company.com"},
    {"Carol White", "4445556666", "carol@company.com"},
    {"David Brown", "7778889999", "david@example.com"},
    {"Eve Davis", "6665554444", "eve@example.com"},
    {"Frank Miller", "3334445555", "frank@example.com"},
    {"Grace Wilson", "2223334444", "grace@example.com"},
    {"Hannah Clark", "5556667777", "hannah@example.com"},
    {"Ian Lewis", "8889990000", "ian@example.com"}
};
void populateAddressBook(AddressBook* addressBook)// Function to populate the address book with dummy contacts
{
    int numDummyContacts = sizeof(dummyContacts) / sizeof(dummyContacts[0]);// Calculate number of dummy contacts
    for (int i = 0; i < numDummyContacts && addressBook->contactCount < MAX_CONTACTS; ++i) // Loop through dummy contacts
    {
        addressBook->contacts[addressBook->contactCount++] = dummyContacts[i];// Add contact to address book and increment count
    }
}