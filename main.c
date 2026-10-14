#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

// Menu definitions
#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

int main(void)
{
    // Variables to intitialize and used throughout the program
    Subsystem subsys;
    SubsystemCollection sub_coll_main, sub_coll_filtered;
    int error, choice, subsys_index;
    unsigned int data;
    unsigned int old_data;
    char status = 0, subsys_name[MAX_STR];
    unsigned char filter_string[MAX_STR], new_status, new_value;

    // Initialize a collection when the program starts
    subsys_collection_init(&sub_coll_main);

    // Set the user user choice before entering the 'do-while' loop
    choice = -1;

    do
    {
        // Print the menu to the user
        print_menu(&choice);

        switch (choice)
        {
        case MENU_ADD:

            // Get the subsystem name from the user and initialize the variable
            add_subsystem_input(subsys_name);
            // Find the index of the subsystem in the collection
            subsys_index = subsys_find(&sub_coll_main, subsys_name);
            // If the subsystem was not found, then intitialize it with the proper variables, and try adding it
            if (subsys_index == ERR_SYS_NOT_FOUND)
            {
                subsys_init(&subsys, subsys_name, status);
                error = subsys_append(&sub_coll_main, &subsys);
                // Print an error if the append was unsuccessful
                print_error(error, NULL, 0);
                // If the collection has capacity, add the subsytem and print to the user that it was added
                if (error != ERR_MAX_CAPACITY)
                {
                    printf("Subsystem '%s' added successfully.\n", subsys_name);
                }
            }
            // If the subsystem was found, then print an error to the user
            else
            {
                printf("Subsystem '%s' already exists.\n", subsys_name);
            }

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            break;

        case MENU_PRINT:

            // Get the name of the subsystem to print from the user and initialize the variable
            print_subsystem_input(subsys_name);
            // Find the index of the subsystem
            subsys_index = subsys_find(&sub_coll_main, subsys_name);
            // If that index exists, then print the subsystem
            if (subsys_index != ERR_SYS_NOT_FOUND)
            {
                // If 'subsys_print()' returns an error, print that error instead
                print_error(subsys_print(&(sub_coll_main.subsystems[subsys_index])), subsys_name, 0);
            }
            // Print that the subsystem was not found
            else
            {
                print_error(subsys_index, subsys_name, 0);
            }

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            break;

        case MENU_PRINTALL:

            // If 'subsys_collection_print()' returns an error (such as an empty collection), print the error, otherwise print all the subsystems
            printf("Subsystems:\n");
            print_error(subsys_collection_print(&sub_coll_main), NULL, 0);

            break;

        case MENU_STATUS:

            // Get the variables to set the status of the subsystem from the user and initialize them
            subsys_status_set_input(subsys_name, &new_status, &new_value);
            // Find the index of the subsystem
            subsys_index = subsys_find(&sub_coll_main, subsys_name);

            // If the index exists, set the status
            if (subsys_index != ERR_SYS_NOT_FOUND)
            {
                error = subsys_status_set(&(sub_coll_main.subsystems[subsys_index]), new_status, new_value);

                // Handle the appropriate error code if the status was unable to be set
                if (error == ERR_INVALID_STATUS)
                {
                    print_error(error, NULL, new_status);
                }
                else if (error == ERR_INVALID_VALUE)
                {
                    print_error(error, NULL, new_value);
                }
                // If the status was set, print a success message to the user
                else
                {
                    printf("Status set successfully.\n");
                }
            }
            // If the index does not exist, then print that the subsystem was not found
            else
            {
                print_error(subsys_index, subsys_name, 0);
            }

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            break;

        case MENU_REMOVE:

            // Get the name of the subsystem to remove from the user and intitalize the variable
            subsys_remove_input(subsys_name);
            // Find the index of the subsystem
            subsys_index = subsys_find(&sub_coll_main, subsys_name);

            // If the index exists, remove the subsystem
            if (subsys_index != ERR_SYS_NOT_FOUND)
            {
                // Remove the subsystem and print a success message to the user
                subsys_remove(&sub_coll_main, subsys_index);
                printf("Subsystem '%s' removed successfully.\n", subsys_name);
            }

            // Print an error if the subsystem was not found
            else
            {
                print_error(subsys_index, subsys_name, 0);
            }

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            break;

        case MENU_FILTER:

            // Get the filter string from the user and initialize the variable
            subsys_filter_input(filter_string);
            // Initialize the filtered collection
            subsys_collection_init(&sub_coll_filtered);
            // Filter the subsystem
            error = subsys_filter(&sub_coll_main, &sub_coll_filtered, filter_string);
            // If the filter string is valid then print the subsystems
            if (error != ERR_INVALID_FILTER_STRING)
            {
                printf("Filtered Subsystems (based on %s):\n", filter_string);
                error = subsys_collection_print(&sub_coll_filtered);
                // If the there are no subsystems based on the filter, then print to the user
                if (error == ERR_EMPTY_COLLECTION)
                {
                    printf("There are no subsystems based on the filter: '%s'\n", filter_string);
                    break;
                }
            }
            // If the filter string is invalid, print an error to the user
            else
            {
                printf("Invalid filter provided.\n");
            }

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            break;

        case MENU_DATA:

            // Get subsystem name and the new data from the user and intialize the variables
            subsys_data_set_input(subsys_name, &data);
            // Find the index of the subsystem
            subsys_index = subsys_find(&sub_coll_main, subsys_name);
            // If the subsystem is found, then set the data
            if (subsys_index != ERR_SYS_NOT_FOUND)
            {
                // Check if there is any data waiting to be printed
                error = subsys_data_get(&(sub_coll_main.subsystems[subsys_index]), &old_data);

                // If there is data, print it before overwriting it
                if (error == ERR_SUCCESS)
                {
                    printf("Previous Data for '%s' is Overwritten: 0x%08X\n", subsys_name, old_data);
                }

                // Set the data based on the user's input
                error = subsys_data_set(&(sub_coll_main.subsystems[subsys_index]), data, &old_data);

                // If the data was set successfully, then print a success message to the user
                if (error == ERR_SUCCESS)
                {
                    printf("Data '0x%08X' for subsystem '%s' set successfully.\n", data, subsys_name);
                }
                // Print the error if data was unable to be set
                else
                {
                    print_error(error, NULL, 0);
                }
            }
            // Print an error if the subsystem was not found
            else
            {
                print_error(subsys_index, subsys_name, 0);
            }

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            break;

        case MENU_EXIT:
            printf("Exiting...\n");
            break;
        }
    } while (choice != MENU_EXIT);
    return 0;
}

int print_menu(int *choice)
{
    if (choice == NULL)
    {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do
    {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n')
            ;

        if (*choice < MENU_MIN || *choice > MENU_MAX)
        {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}

int print_error(int error_code, char *details_string, int details_int)
{
    if (details_string == NULL)
    {
        details_string = "";
    }

    switch (error_code)
    {
    case ERR_SUCCESS:
        break;
    case ERR_INVALID_INDEX:
        printf("Error: Invalid index provided.\n");
        break;
    case ERR_NO_DATA:
        printf("Error: No data in the subsystem.\n");
        break;
    case ERR_INVALID_STATUS:
        printf("Error: Invalid status '%d' provided\n", details_int);
        break;
    case ERR_INVALID_VALUE:
        printf("Error: Invalid value '%d' provided.\n", details_int);
        break;
    case ERR_MAX_CAPACITY:
        printf("Error: Max subsystem collection capacity reached.\n");
        break;
    case ERR_NULL_POINTER:
        printf("Error: Null pointer passed as an argument.\n");
        break;
    case ERR_SYS_NOT_FOUND:
        printf("Error: The subsystem with the name '%s' was not found.\n", details_string);
        break;
    case ERR_EMPTY_COLLECTION:
        printf("Error: The subsystem collection is empty.\n");
        break;
    default:
        printf("Error: Unknown.\n");
        break;
    }
    return error_code;
}
