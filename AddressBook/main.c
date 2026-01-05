/*
NAME        :   Kishore
DATE        :   12-12-2025
PROJECT     :   Adress Book
DESCRIPTION :   Menu-driven program to manage contacts with create, search,
                edit, delete, list, and save options.
*/

//include necessary standard header files
#include <stdio.h>
//include custom header file for address book functionalities
#include "contact.h"// Include the contact header file for address book functionalities
#include "ui.h"
int main()
{// Function to run the address book application
    int choice;// Variable to store user choice
    AddressBook addressBook;// Declare an AddressBook instance
    initialize(&addressBook); // Initialize the address book
   // Main loop for the address book menu
    do 
    {
        printf(CYAN "\n========= ADDRESS BOOK MENU =========\n" RESET);// Display menu options
        printf(YELLOW"1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save contacts\n");		
        printf("7. Exit\n"RESET);
        printf("Enter your choice: ");
        int x=scanf("%i", &choice);// Get user choice
        getchar();// Consume newline character left by scanf
        if(x!=1)// Validate user input
        {
            printf(RED"🤪 Invalid input. Please enter a number between 1 and 7.\n"RESET);
            while(getchar()!='\n');// Clear input buffer
            continue;// Restart loop for valid input
        }
        // Handle user choice using switch-case
        switch (choice)
        {
            // Handle user choices
            case 1:
            createContact(&addressBook);// Call function to create a new contact
            break;
            case 2:
            searchContact(&addressBook);// Call function to search for a contact
            break;
            case 3:
            editContact(&addressBook);// Call function to edit a contact
            break;
            case 4:
            deleteContact(&addressBook);// Call function to delete a contact
            break;
            case 5:          
            listContacts(&addressBook);// Call function to list all contacts
            break;
            case 6:
            printf(GREEN"Saving...🤝\n"RESET);
            save(&addressBook);// Call function to save contacts to file  
            break;
            case 7:
            save(&addressBook);// Call function to save contacts to file  
            printf(GREEN"Exiting...Bye....👋\n"RESET);// Exit the program
            break;
            default:
            printf(RED" Invalid choice. Please try again.\n"RESET);// Handle invalid choices
        }// End of switch
    }// End of do-while loop 
    while (choice != 7);// Continue until user chooses to exit
       return 0;
}
