/*
NAME: Kishore
DATE: 25-03-2026
PROJECT NAME: Inverted Search Engine
DESCRIPTION:
		 This program implements an inverted search engine that allows users to
		  create, display, save, search, and update a database of words and their
		   occurrences in text files. 
*/
#include "inverted.h"
//Variables to track database creation and update status
int db_created = 0;
int db_updated = 0;
// Function to free all allocated memory for the database
void free_database(M_node *HT[])
{
    for(int i = 0; i < 27; i++)//Traverse each index of hash table
    {
        M_node *mtemp = HT[i];
        while(mtemp)//Traverse main node list
        {
            S_node *stemp = mtemp->sub_link;
            while(stemp)//Traverse sub node list and free each sub node
            {
                S_node *sdel = stemp;
                stemp = stemp->sub_link;
                free(sdel);//Free sub node
            }

            M_node *mdel = mtemp;
            mtemp = mtemp->main_link;
            free(mdel);//Free main node
        }
    }
}
int main(int argc, char *argv[])
{
    F_node *head = NULL;
    M_node *HT[27];
	for(int i = 0; i < 27; i++)//Initialize hash table with NULL
        HT[i] = NULL;
	if(argc<2)
	{
		printf("INFO: Insufficient arguments\nPlease pass the arguments like ./a.out <txt file>\n");
		return 0;
	}
    // Validate_files
	validate_files(argv, &head);
    int option;
    do
    {
	printf("Select your choice among following operations:\n1. Create Database\n2. Display Database\n3. Save Database\n4. Search\n5. Update Database\n6. Exit\n\nEnter your choice : ");
	int x = scanf("%d", &option);
	// Clear input buffer if scanf fails to read an integer
	if (x != 1)
	{
		printf("INFO : Please enter a valid numeric option (1-6)\n");
		while (getchar() != '\n');
		continue;
	}
	// Handle user options
	switch (option)
	{
		case 1: // CREATE
			if (!db_created)//Allow creation only if not already created
			{
				create_database(HT, head);
				db_created = 1;
			}
			else
			{
				printf("Database already created\n");
			}
			break;
		case 2:
			if (db_created || db_updated)//Allow display if database is created or updated
				display_database(HT);
			else 
				printf("Database not created\n");
			break;
		case 3:
			if (db_created || db_updated)//Allow save if database is created or updated
				save_database(HT);
			else 
				printf("Database not created\n");
			break;
		case 4:
			if (db_created || db_updated)//Allow search if database is created or updated
				search_database(HT);
			else 
				printf("Database not created\n");
			break;
		case 5: // UPDATE
			if (db_created)//Disallow update if database is already created
			{
				printf("Cannot update after creation\n");
			}
			else if (!db_updated)//Allow update only if not already updated
			{
				update_database(HT, &head);
				db_updated = 1;
			}
			else
			{
				printf("Database already updated\n");
			}
			break;
		case 6:
			free_database(HT);//Free all allocated memory before exiting
			printf("Exiting...\n");
			break;
		default:
			printf("INFO : Please enter the valid option\n");
	}
    }while( option != 6 );
    return 0;
}