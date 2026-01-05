//include necessary standard header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//include custom header files
#include "contact.h"// Include the contact header file for address book functionalities
#include "file.h" // Include the file header file for file operations
#include "validate.h" // Include the validate header file for input validation
#include "ui.h"
AddressBook addressBook;// Declare an AddressBook instance
void print()
{
    int i=0;
    while(i++ <50)
    printf("-");
    printf("\n");
}
void listContacts(AddressBook *addressBook) //
{
    sortContacts(addressBook);
    printf(BLUE "\n================= CONTACT LIST =================\n\n" RESET);
    // Display all contacts
    if (addressBook->contactCount == 0)
    {
        printf(RED "🤷 No contacts available.\n" RESET);
        return;
    }
    for(int i=0;i < addressBook->contactCount;i++)// Loop through each contact
    {
        printf(GREEN"\n%d.\n"RESET,i+1);// Display contact number
        print();
        printf("* "MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
        printf("* "MAGENTA "Phone No.: " RESET "%s\n"RESET,addressBook->contacts[i].phone);// Display contact phone number
        printf("* "MAGENTA "Mail ID  : " RESET "%s\n"RESET,addressBook->contacts[i].mail);// Display contact mail ID
        print();
    }   
}
void initialize(AddressBook *addressBook) // Function to initialize the address book
{
    addressBook->contactCount = 0;// Set initial contact count to 0
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
    printf(GREEN "Address Book Loaded Successfully!\n" RESET);

}
void save(AddressBook *addressBook) // Function to save contacts and exit the program
{
    sortContacts(addressBook);//Sort Contacts
    saveContactsToFile(addressBook); // Save contacts to file
    printf(GREEN "😁 Contacts Saved Successfully!\n" RESET);
}
void createContact(AddressBook *addressBook) // Function to create a new contact
{

    Contact new;// Declare a new Contact instance
    printf(CYAN "\n=========== CREATE NEW CONTACT ===========\n" RESET);
    // Get contact details from user
   do // Loop to get valid name
    {
        printf(YELLOW"Enter Name : "RESET);// Prompt user for contact name
        scanf("%[^\n]",new.name);// Read contact name
        getchar();// Consume newline character
        if(!validatename(new.name))// Validate contact name
            printf(RED"❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
    }// End of do-while loop
    while (!validatename(new.name));// || !checkDuplicatename(new.name,addressBook));// Continue until valid and unique name is provided
    do // Loop to get valid phone number
    {
        printf(YELLOW"Enter Phone no. : "RESET);// Prompt user for contact phone number
        scanf("%[^\n]",new.phone);// Read contact phone number
        getchar();// Consume newline character
        if(!validateno(new.phone))// Validate contact phone number
            printf(RED"❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
        if(!checkDuplicateno(new.phone,addressBook))// Check for duplicate contact phone number
            printf(RED"❌ 😎 Number Already exists!\n"RESET);// Prompt user if phone number already exists
    }// End of do-while loop
    while (!validateno(new.phone) || !checkDuplicateno(new.phone,addressBook));// Continue until valid and unique phone number is provided
    do // Loop to get valid mail ID
    {
        printf(YELLOW"Enter Mail ID : "RESET);// Prompt user for contact mail ID
        scanf("%[^\n]",new.mail);// Read contact mail ID
        getchar();// Consume newline character
        if(!validatmail(new.mail))// Validate contact mail ID
            printf(RED"❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
        if(!checkDuplicatmail(new.mail,addressBook))// Check for duplicate contact mail ID
            printf(RED"❌ 😎 Mail Already exists!\n"RESET);// Prompt user if mail ID already exists
    }// End of do-while loop
    while (!validatmail(new.mail) || !checkDuplicatmail(new.mail,addressBook));// Continue until valid and unique mail ID is provided
    // Display success message
    printf(GREEN "\n✔ Contact Successfully Created!\n"RESET);
    printf(GREEN" 😉 Save before exiting!\n"RESET);// Prompt user to save before exiting
    // Add the new contact to the address book
    strcpy(addressBook->contacts[addressBook->contactCount].name,new.name);// Copy contact name to address book
    strcpy(addressBook->contacts[addressBook->contactCount].phone,new.phone);// Copy contact phone number to address book
    strcpy(addressBook->contacts[addressBook->contactCount].mail,new.mail);// Copy contact mail ID to address book
    addressBook->contactCount++;// Increment contact count
    printf(CYAN"Total Contacts Count=%d\n",addressBook->contactCount); // Display total contact count
}
void searchContact(AddressBook *addressBook) // Function to search for a contact
{
    // Define the logic for searchcontact
    if (addressBook->contactCount == 0)
    {
        printf(RED "🤷‍ No contacts available.\n" RESET);
        return;
    }
    //listContacts(addressBook);// List all contacts
    int ch;// Variable to store user choice
    printf(CYAN "\n=============== SEARCH MENU ===============\n" RESET);
    do// Loop to get valid search option
    {
        // Display search options
        printf(YELLOW"1.Search by Name\n");// Display search by name option
        printf("2.Search by Phone Number\n");// Display search by phone number option
        printf("3.Search by Mail ID\n");// Display search by mail ID option
        printf("4.Exit Search\n"RESET);// Display exit option
        printf("Choose an Option: ");// Prompt user to choose an option
        // int y = scanf("%d",&ch);// Read user choice
        // getchar();// Consume newline character  
        if(scanf("%d",&ch)!=1)// If input is not a valid integer
        {
            printf(RED "❌ Error!🤪 Invalid Choose Correct Option\n"RESET);// Display error message
            while(getchar()!='\n');// Clear input buffer
                continue;// Continue to next iteration of loop
        }
        getchar();
        switch(ch)// Handle user choice
        {
            case 1: 
            case 2: 
            case 3: 
            search(addressBook,ch);// Call search function based on user choice
            break;
            case 4: 
            break;// Exit option
            default:
            printf(RED "❌ Error!🤪 Choose Correct Option\n"RESET);// Handle🤪 Invalid choice
            break;
        }
    }// End of do-while loop
    while(ch!=4);// Continue until user chooses to exit
}
void search(AddressBook *addressBook,int x)// Function to search for a contact by name, phone number, or mail ID
{
    if(x==1)// Search by name
    {
        char name[40];// Variable to store contact name
        int count=0;// Variable to count number of matches
        printf("Enter Name : ");// Prompt user for contact name
        scanf("%[^\n]",name);// Read contact name
        getchar();// Consume newline character
        printf(CYAN "\n=========== SEARCH RESULTS ===========\n" RESET);
        for(int i=0;i<addressBook->contactCount;i++)// Loop through each contact
        {
            if(strcmp(name,addressBook->contacts[i].name)==0)// Compare contact name with user input
            {
                count++;// Increment match count
                printf(GREEN "%d.\n" RESET,count);// Display match number
                printf(MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
                printf(MAGENTA "Phone No.: " RESET "%s\n",addressBook->contacts[i].phone);// Display contact phone number
                printf(MAGENTA "Mail ID  : " RESET "%s\n",addressBook->contacts[i].mail);// Display contact mail ID
                printf(YELLOW "Index=%d\n\n" RESET ,i+1);// Display contact index in address book
            }
        }
        if(count==0)// If no matches found
        printf(RED "❌ No matching record found.\n" RESET);// Display not found message
        if(count > 1)
        {
            printf(RED " Multiple contacts found with the same name. Choose other Option\n"RESET);
        }
    }
    else if(x==2)// Search by phone number
    {
        char phone[40];// Variable to store contact phone number
        printf("Enter Number : ");// Prompt user for contact phone number
        scanf("%[^\n]",phone);// Read contact phone number
        getchar();// Consume newline character
        printf(CYAN "\n=========== SEARCH RESULTS ===========\n" RESET);
        int count=0;// Variable to count number of matches
        for(int i=0;i<addressBook->contactCount;i++)// Loop through each contact
        {
            if(strcmp(phone,addressBook->contacts[i].phone)==0)// Compare contact phone number with user input
            {
                count++;// Increment match count
                printf(GREEN "%d.\n" RESET,count);// Display match number
                printf(MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
                printf(MAGENTA "Phone No.: " RESET "%s\n",addressBook->contacts[i].phone);// Display contact phone number
                printf(MAGENTA "Mail ID  : " RESET "%s\n",addressBook->contacts[i].mail);// Display contact mail ID
                printf(YELLOW "Index=%d\n\n" RESET ,i+1);// Display contact index in address book
            }
        }
        if(count==0)// If no matches found
        printf(RED "❌ No matching record found.\n" RESET);// Display not found message
    }
    else// Search by mail ID
    {
        char mail[40];// Variable to store contact mail ID
        printf("Enter Mail ID : ");// Prompt user for contact mail ID
        scanf("%[^\n]",mail);// Read contact mail ID
        getchar();// Consume newline character
        printf(CYAN "\n=========== SEARCH RESULTS ===========\n" RESET);
        int count=0;// Variable to count number of matches
        for(int i=0;i<addressBook->contactCount;i++)// Loop through each contact
        {
            if(strcmp(mail,addressBook->contacts[i].mail)==0)// Compare contact mail ID with user input
            {
                count++;// Increment match count
                printf(GREEN "%d.\n" RESET,count);// Display match number
                printf(MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
                printf(MAGENTA "Phone No.: " RESET "%s\n",addressBook->contacts[i].phone);// Display contact phone number
                printf(MAGENTA "Mail ID  : " RESET "%s\n",addressBook->contacts[i].mail);// Display contact mail ID
                printf(YELLOW "Index=%d\n\n" RESET ,i+1);// Display contact index in address book
            }
        }
        if(count==0)// If no matches found
        printf(RED "❌ No matching record found.\n" RESET);// Display not found message
    }// End of search function
}
void editContact(AddressBook *addressBook) // Function to edit an existing contact
{
    if (addressBook->contactCount == 0)
    {
        printf(RED "🤷‍ No contacts available.\n" RESET);
        return;
    }
    listContacts(addressBook);// List all contacts
    int ch,i;// Variables to store user choice
    printf(YELLOW"Enter the index of contact to edit: "RESET);// Prompt user for contact index
    scanf("%d",&ch);// Read contact index
    getchar();// Consume newline character
    if (ch < 1 || ch > addressBook->contactCount)
    {
        printf(RED "❌🤪 Invalid index!\n" RESET);
        return;
    }
    Contact new;// Declare a new Contact instance to store updated details
    // Get updated contact details from user
    do// Loop to get valid edit option
    {
        printf(CYAN "\n=========== EDIT MENU ===========\n" RESET);
        printf(YELLOW"1.Edit Name\n2.Edit Number\n3.Edit Mail ID\n4.Exit Edit\n"RESET);// Display edit options
        printf("Choose an Option: ");// Prompt user for edit option
        scanf("%d",&i);// Read edit option
        getchar();// Consume newline character
        // Handle user choice for editing contact details
        switch(i)// Switch case for edit options
        {
            case 1:// Edit contact name
                do // Loop to get valid name
                {
                    printf(MAGENTA"Old Name : "RESET"%s\n",addressBook->contacts[ch-1].name);//Display Old name of selected Contact
                    printf(MAGENTA"Enter New Name :"RESET);// Prompt user for new contact name
                    scanf("%[^\n]",new.name);// Read new contact name
                    getchar();// Consume newline character
                    if(!validatename(new.name))// Validate new contact name
                        printf(RED "❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
                }
                while (!validatename(new.name));// || !checkDuplicatename(new.name,addressBook));// Continue until valid name is provided
                strcpy(addressBook->contacts[ch-1].name,new.name);// Copy new contact name to address book
                printf(GREEN "✔ Name Successfully Updated!\nSave before exiting!\n");// Prompt user to save before exiting
            break;// End of edit name case
            case 2:// Edit contact phone number
                do // Loop to get valid phone number
                {
                    printf(MAGENTA"Old Number : "RESET"%s\n",addressBook->contacts[ch-1].phone);//Display Old Number of selected Contact
                    printf(MAGENTA"Enter New Number :"RESET);// Prompt user for new contact phone number
                    scanf("%[^\n]",new.phone);// Read new contact phone number
                    getchar();// Consume newline character
                    if(!validateno(new.phone))// Validate new contact phone number
                        printf(RED "❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
                    if(!checkDuplicateno(new.phone,addressBook))// Check for duplicate contact phone number
                        printf(RED"❌ 😎 Number Already exists!\n"RESET);// Prompt user if phone number already exists
                }
                while (!validateno(new.phone) || !checkDuplicateno(new.phone,addressBook));// Continue until valid phone number is provided
                strcpy(addressBook->contacts[ch-1].phone,new.phone);// Copy new contact phone number to address book
                printf(GREEN "✔ Number Successfully Updated!\nSave before exiting!\n");// Prompt user to save before exiting
            break;// End of edit phone number case
            case 3:// Edit contact mail ID
                do // Loop to get valid mail ID
                {
                    printf(MAGENTA"Old Mail-ID : "RESET"%s\n",addressBook->contacts[ch-1].mail);//Display Old Mail of selected Contact
                    printf(MAGENTA"Enter New Mail-ID :"RESET);// Prompt user for new contact mail ID
                    scanf("%[^\n]",new.mail);// Read new contact mail ID
                    getchar();// Consume newline character
                    if(!validatmail(new.mail))// Validate new contact mail ID
                        printf(RED "❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
                    if(!checkDuplicatmail(new.mail,addressBook))// Check for duplicate contact mail ID
                        printf(RED"❌ 😎 Mail Already exists!\n"RESET);// Prompt user if mail ID already exists
                }
                while (!validatmail(new.mail) || !checkDuplicatmail(new.mail,addressBook));// Continue until valid mail ID is provided
                strcpy(addressBook->contacts[ch-1].mail,new.mail);// Copy new contact mail ID to address book
                printf(GREEN "✔ E-Mail Successfully Updated!\nSave before exiting!\n");// Prompt user to save before exiting
            break;// End of edit mail ID case
            case 4: 
            break;
            default:// Handle🤪 Invalid choice
            printf(RED "❌🤪 Invalid Option!\n"RESET);// Prompt user for🤪 Invalid option
            break;// End of default case
        }// End of switch case
    }// End of do-while loop
    while(i!=4);// Continue until user chooses to exit
}
void deleteContact(AddressBook *addressBook) // Function to edit an existing contact
{
    if (addressBook->contactCount == 0)
    {
        printf(RED "🤷‍ No contacts available.\n" RESET);
        return;
    }
    //listContacts(addressBook);// List all contacts
    Contact new;// Declare a new Contact instance to store updated details
    int i;// Variables to store user choice

    do
    {
        printf(CYAN "\n=========== DELETE MENU ===========\n" RESET);
        printf(YELLOW"\n1.Delete by Name\n2.Delete by Number\n3.Delete by Mail ID\n4.Exit Delete\n"RESET);// Display edit options
        printf("Choose an Option: ");// Prompt user for delete option
        
        if(scanf("%d",&i)!=1)// If input is not a valid integer
        {
            printf(RED"Error!🤪 Invalid input\n"RESET);// Display error message
            while(getchar()!='\n');// Clear input buffer
            continue;// Continue to next iteration of loop
        }
        getchar();

        // Handle user choice for deleting contact details
        switch(i)// Switch case for edit options
        {
            case 1:// delete contact name
            {
                do // Loop to get valid name
                {
                    printf("Enter Name : ");// Prompt user for contact name
                    scanf("%[^\n]",new.name);// Read contact name
                    getchar();// Consume newline character
                    if(!validatename(new.name))// Validate new contact name
                        printf(RED "❌Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
                }
                while (!validatename(new.name));// Continue until valid name is provided

                int ch1 = -1, count1 = 0;// Variable to store contact index
                int matchIndexes[200]; // store indexes of all matches

                for(int i=0;i < addressBook->contactCount;i++)// Loop through each contact
                {
                    if(strcmp(new.name,addressBook->contacts[i].name)==0)// Compare contact name with user input
                    {
                        matchIndexes[count1] = i;
                        count1++;// Increment match count

                        printf(GREEN"\n%d.\n"RESET, i+1);// Display contact number
                        print();
                        printf("* "MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
                        printf("* "MAGENTA "Phone No.: " RESET "%s\n",addressBook->contacts[i].phone);// Display contact phone number
                        printf("* "MAGENTA "Mail ID  : " RESET "%s\n",addressBook->contacts[i].mail);// Display contact mail ID
                        print();
                    }
                }

                if(count1 == 0)
                {
                    printf(RED "❌Name not found!\n"RESET);
                    break;
                }

                if(count1 > 1)
                {
                    printf(RED "❌ Multiple entries found with the same name. Choose other Option\n"RESET);
                    break;
                }

                // exactly one match
                ch1 = matchIndexes[0];

                int confirm;
                printf(YELLOW"Are You Sure?\n"RESET GREEN"1.Yes\n"RESET RED"2.No\n"RESET);
                printf("Choose your option : ");
                if(scanf("%d",&confirm)!=1)// If input is not a valid integer
                {
                    printf(RED "❌ Error!🤪 Invalid Choose Correct Option\n"RESET);// Display error message
                    while(getchar()!='\n');// Clear input buffer
                    continue;// Continue to next iteration of loop
                }

                if(confirm==1)
                {
                    for(int i=ch1;i < addressBook->contactCount-1;i++)// Loop through contacts starting from the deleted contact index
                    {
                        // Shift contacts up to fill the gap left by the deleted contact
                        addressBook->contacts[i] = addressBook->contacts[i+1];
                    }
                    addressBook->contactCount--;// Decrement contact count after deletion
                    printf(GREEN "\n✔ Contact Successfully Deleted!\nSave before exiting!\n");
                }
                break;// End of delete name case
            }

            case 2:// Delete contact phone number
            {
                do // Loop to get valid phone number
                {
                    printf("Enter Phone no. : ");// Prompt user for contact phone number
                    scanf("%[^\n]",new.phone);// Read contact phone number
                    getchar();// Consume newline character
                    if(!validateno(new.phone))// Validate contact phone number
                        printf(RED "❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
                }
                while (!validateno(new.phone));// Continue until valid phone number is provided

                int ch2 = -1, count2=0;// Variable to store contact index

                for(int i=0;i < addressBook->contactCount;i++)// Loop through each contact
                {   
                    if(strcmp(new.phone,addressBook->contacts[i].phone)==0)// Compare contact phone number with user input
                    {
                        count2=1;// Increment match count
                        ch2=i;// Store contact index
                        printf(GREEN"\n%d.\n"RESET,i+1);// Display contact number
                        print();
                        printf("* "MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
                        printf("* "MAGENTA "Phone No.: " RESET "%s\n",addressBook->contacts[i].phone);// Display contact phone number
                        printf("* "MAGENTA "Mail ID  : " RESET "%s\n",addressBook->contacts[i].mail);// Display contact mail ID
                        print();
                        break;// Exit loop after finding the contact
                    }
                }

                if(count2==0)
                {
                    printf(RED "❌ Number not found!\n"RESET);
                    break;
                }

                int confirm;
                printf(YELLOW"Are You Sure?\n"RESET GREEN"1.Yes\n"RESET RED"2.No\n"RESET);
                printf("Choose your option : ");
                if(scanf("%d",&confirm)!=1)// If input is not a valid integer
                {
                    printf(RED "❌ Error!🤪 Invalid Choose Correct Option\n"RESET);// Display error message
                    while(getchar()!='\n');// Clear input buffer
                    continue;// Continue to next iteration of loop
                }

                if(confirm==1)
                {
                    for(int i=ch2;i < addressBook->contactCount-1;i++)// Loop through contacts starting from the deleted contact index
                    {
                        addressBook->contacts[i] = addressBook->contacts[i+1];
                    }
                    addressBook->contactCount--;// Decrement contact count after deletion
                    printf(GREEN "\n✔ Contact Successfully Deleted!\nSave before exiting!\n");
                }
                break;// End of delete phone number case
            }

            case 3:// Delete contact mail ID
            {
                do // Loop to get valid mail ID
                {
                    printf("Enter Mail ID : ");// Prompt user for contact mail ID
                    scanf("%[^\n]",new.mail);// Read contact mail ID
                    getchar();// Consume newline character
                    if(!validatmail(new.mail))// Validate contact mail ID
                        printf(RED "❌🤪 Invalid. Try again!\n"RESET);// Prompt user to try again if🤪 Invalid
                }
                while (!validatmail(new.mail));// Continue until valid mail ID is provided

                int ch3 = -1, count3 = 0;// Variable to store contact index

                for(int i=0;i < addressBook->contactCount;i++)// Loop through each contact
                {
                    if(strcmp(new.mail,addressBook->contacts[i].mail)==0)// Compare contact mail ID with user input
                    {
                        count3=1;
                        ch3=i;// Store contact index

                        printf(GREEN"\n%d.\n"RESET,i+1);// Display contact number
                        print();
                        printf("* "MAGENTA "Name     : " RESET "%s\n",addressBook->contacts[i].name);// Display contact name
                        printf("* "MAGENTA "Phone No.: " RESET "%s\n",addressBook->contacts[i].phone);// Display contact phone number
                        printf("* "MAGENTA "Mail ID  : " RESET "%s\n",addressBook->contacts[i].mail);// Display contact mail ID
                        print();
                        break;
                    }
                }

                if(count3==0)
                {
                    printf(RED "❌ Mail ID not found!\n"RESET);
                    break;
                }

                int confirm;
                printf(YELLOW"Are You Sure?\n"RESET GREEN"1.Yes\n"RESET RED"2.No\n"RESET);
                printf("Choose your option : ");
                if(scanf("%d",&confirm)!=1)// If input is not a valid integer
                {
                    printf(RED "❌ Error!🤪 Invalid Choose Correct Option\n"RESET);// Display error message
                    while(getchar()!='\n');// Clear input buffer
                    continue;// Continue to next iteration of loop
                }

                if(confirm==1)
                {
                    for(int i=ch3;i < addressBook->contactCount-1;i++)// Loop through contacts starting from the deleted contact index
                    {
                        addressBook->contacts[i] = addressBook->contacts[i+1];
                    }
                    addressBook->contactCount--;// Decrement contact count after deletion
                    printf(GREEN "\n✔ Contact Successfully Deleted!\nSave before exiting!\n");
                }
                break;// End of delete mail ID case
            }

            case 4: 
                break;// Exit option

            default:// Handle🤪 Invalid choice
                printf(RED "❌🤪 Invalid Option!\n"RESET);// Prompt user for🤪 Invalid option
                break;// End of default case
        }// End of switch case
    }// End of do-while loop
    while(i!=4);// Continue until user chooses to exit
}
