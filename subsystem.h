#include <stdio.h>
#include <string.h>

// Error Codes
#define ERR_SUCCESS 0
#define ERR_INVALID_INDEX -1
#define ERR_NO_DATA -2
#define ERR_INVALID_STATUS -3
#define ERR_MAX_CAPACITY -4
#define ERR_NULL_POINTER -5
#define ERR_SYS_NOT_FOUND -6

// Additional Error Codes
#define ERR_EMPTY_COLLECTION -7
#define ERR_INVALID_VALUE -8
#define ERR_INVALID_FILTER_STRING -9

// Status Bits
#define STATUS_POWER 7
#define STATUS_DATA 6
#define STATUS_ACTIVITY 5
#define STATUS_ERROR 4
#define STATUS_PERFORMANCE 2
#define STATUS_RESOURCE 0

// Magic Numbers
#define MAX_STR 32
#define MAX_ARR 100

// Subsystem Structure
typedef struct
{
    char name[MAX_STR];
    unsigned char status;
    unsigned int data;
} Subsystem;

// Subsystem Collection Structure
typedef struct
{
    Subsystem subsystems[MAX_ARR];
    unsigned int size;
} SubsystemCollection;

/*

Forward Declarations for 'subsys_collection.c'

*/

/*
   Initializes the subsystem collection.

   in/out *subsystems: Pointer to the SubsystemCollection to initialize.

   Returns:
       ERR_SUCCESS if successful.
*/
int subsys_collection_init(SubsystemCollection *subsystems);

/*
   Appends a new subsystem to the collection.

    in/out *subsystems: Pointer to the SubsystemCollection to append to.
    in      *subsystem: Pointer to the Subsystem to append.

   Returns:
       ERR_MAX_CAPACITY: If the collection has reached its maximum size.
       ERR_SUCCESS: Successfully appended the subsystem.
*/
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem);

/*
   Finds a subsystem by name in the collection.

   in *subsystems: Pointer to the SubsystemCollection to search
   in       *name: The name of the subsystem to find

   Returns:
       ERR_SYS_NOT_FOUND: If the subsystem is not found
       The index of the subsystem in the collection if found
*/
int subsys_find(const SubsystemCollection *subsystems, const char *name);

/*
   Prints all subsystems in the collection.

   in *subsystems: Pointer to the SubsystemCollection to print.

   Returns:
       ERR_EMPTY_COLLECTION: If the collection is empty.
       ERR_SUCCESS: Successfully printed the subsystems.
*/
int subsys_collection_print(SubsystemCollection *subsystems);

/*
   Removes a subsystem from the collection by index

   in/out *subsystems: Pointer to the SubsystemCollection to remove
   in           index: The index of the subsystem to remove

   Returns:
       ERR_NO_DATA: If there is no data in the collection.
       ERR_SUCCESS: Successfully removed the subsystem.
       ERR_INVALID_INDEX: If the provided index is invalid.
*/
int subsys_remove(SubsystemCollection *subsystems, int index);

/*
   Filters subsystems based on a filter mask.

   in      *src: Pointer to the source SubsystemCollection to filter.
   in   *filter: The filter mask to apply.
   in/out *dest: Pointer to the destination SubsystemCollection.

   Returns:
       - ERR_SUCESS if successfully filtered the subsystems.
*/
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter);

/*

Forward Declarations for 'subsys.c'

*/

/*
    Initializes a subsystem with a given name and status.

    in/out *subsystem: Pointer to the Subsystem to initialize.
    in          *name: The name to set for the subsystem.
    in         status: The status to set for the subsystem.

    Returns:
        ERR_SUCCESS: Successfully initialized the subsystem.
*/
int subsys_init(Subsystem *subsystem, const char *name, char status);

/*
    Prints the details of a subsystem including its name, status, and data.

    in *subsystem: Pointer to the Subsystem to print.

    Returns:
        ERR_NULL_POINTER: If the subsystem pointer is NULL.
        ERR_SUCCESS: Successfully printed the subsystem details.
*/
int subsys_print(Subsystem *subsystem);

/*
    Sets the status of a subsystem at a given position with a specified value.

    in/out *subsystem: Pointer to the Subsystem to update.
    in         status: The position in the status byte to modify.
    in          value: The value to set at the specified status position.

    Returns:
        ERR_NULL_POINTER: If the subsystem pointer is NULL.
        ERR_INVALID_VALUE: If an invalid value is provided for the status.
        ERR_INVALID_STATUS: If an invalid status is specified.
        ERR_SUCCESS: Successfully set the status.
*/
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value);

/*
    Prints the status of a subsystem in a human-readable format.

    in *subsystem: Pointer to the Subsystem to print the status of.

    Returns:
        ERR_NULL_POINTER: If the subsystem pointer is NULL.
        ERR_SUCCESS: Successfully printed the subsystem status.
*/
int subsys_status_print(const Subsystem *subsystem);

/*
    Sets new data for a subsystem, and stores the previous data

    in/out *subsystem: Pointer to the Subsystem to update
    in       new_data: The new data to set
    in/out  *old_data: Pointer to store the old data before updating

    Returns:
        ERR_NULL_POINTER: If any of the pointers are NULL
        ERR_SUCCESS: Successfully set the new data and stored the old data
*/
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data);

/*
    Retrieves the data of a subsystem and resets the status.

    in/out *subsystem: Pointer to the Subsystem to retrieve data from.
    in/out      *data: Pointer to store the retrieved data.

    Returns:
        ERR_NULL_POINTER: If the subsystem pointer is NULL.
        ERR_NO_DATA: If no data is available in the subsystem.
        ERR_SUCCESS: Successfully retrieved the data.
*/
int subsys_data_get(Subsystem *subsystem, unsigned int *data);

/*

Forward Declarations for 'main.c'

*/

/*
    Prints a menu to the user and prompts them until they enter a valid menu option.

    in/out *choice: an integer defining the choice made by the user.

    Returns:
        - ERR_SUCCESS if successful
        - ERR_NULL_POINTER if 'choice' is NULL
*/
int print_menu(int *choice);

/*
    Prompts the user to enter a new subsystem name

    out *name: The name of the new subsystem

    Returns:
        - ERR_SUCCESS if successfull
*/
int add_subsystem_input(char *name);

/*
    Prompts the user for a subsystem name to print

    out *name: The name of the subsystem to print

    Returns:
        - ERR_SUCCESS if successfull
*/
int print_subsystem_input(char *name);

/*
    Prompts the user to set a subsystem's status and value

    in/out   *name: The name of the subsystem
    in/out *status: The status ID to set
    in/out  *value: The new value to assign

    Returns:
        - ERR_SUCCESS if successfull
*/
int subsys_status_set_input(char *name, unsigned char *status, unsigned char *value);

/*
    Prompts the user for a subsystem name and data value.

    in/out *subsys_name: The name of the subsystem.
    in/out        *data: The new data value in hex format

    Returns:
        - ERR_SUCCESS if successful
*/
int subsys_data_set_input(char *name, unsigned int *data);

/*
    Prompts the user for a subsystem name to remove.

    in *subsys_name: The name of the subsystem to be removed.

    Returns:
        - ERR_SUCCESS if successful
*/
int subsys_remove_input(char *subsys_name);

/*
    Prompts the user for a 'filter' string to filter out the collections based on the input.

    in *filter_string: String to filter the subsystem collection.

    Returns:
        - ERR_SUCCESS if successful
*/
int subsys_filter_input(unsigned char *filter_string);

/*
    Prints an error message based on error code

    in      error_code: The error code
    in *details_string: String details
    in     details_int: Integer details

    Returns:
        - The error code provided as input
*/
int print_error(int error_code, char *details_string, int details_int);
