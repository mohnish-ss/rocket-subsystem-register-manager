#include "subsystem.h"
#include <string.h>
#include <strings.h>

int subsys_collection_init(SubsystemCollection *subsystems)
{
    // Check if the subsystems collection is null, if so return null pointer
    if (subsystems == NULL)
    {
        return ERR_NULL_POINTER;
    }

    // Set the subsystem size to 0 for intialization
    subsystems->size = 0;

    return ERR_SUCCESS;
}

int subsys_collection_print(SubsystemCollection *subsystems)
{
    // Create a size variable and set it to the size field of the subsystems
    int size = subsystems->size;

    // Check whether or not the subsystems is empty
    if (size == 0)
    {
        return ERR_EMPTY_COLLECTION;
    }

    // Iterate through the subsystems in order to print all of them
    for (int i = 0; i < size; i++)
    {
        // Call the singluar print method and pass in the adrress of each subsystem
        subsys_print(&(subsystems->subsystems[i]));
    }

    return ERR_SUCCESS;
}

int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem)
{
    // Create a subsystem
    Subsystem sub;

    // Check whether or not the subsystems collection has space
    if (subsystems->size < MAX_ARR)
    {
        // Set the sub variable to the value of the subsystem pointer coming in
        sub = *subsystem;

        // Assign the sub to the next available position in the subsystems array
        subsystems->subsystems[subsystems->size] = sub;

        // Increment the size field of the subsystems collection
        subsystems->size++;

        return ERR_SUCCESS;
    }

    return ERR_MAX_CAPACITY;
}

int subsys_remove(SubsystemCollection *subsystems, int index)
{
    // If the size of the subsytems is equal to zero, send a no data error
    if (subsystems->size == 0)
    {
        return ERR_NO_DATA;
    }

    // If the index is less then 0, or index is greater then the subsystems size field
    if (index < 0 || (unsigned int)index >= subsystems->size)
    {
        return ERR_INVALID_INDEX;
    }

    // Set the size variable to the size
    int size = subsystems->size - 1;

    // Itereate through each subsystem and move it to the left (deletes the subsystem at the index)
    for (int i = index; i < size; i++)
    {
        subsystems->subsystems[i] = subsystems->subsystems[i + 1];
    }

    subsystems->size--;

    return ERR_SUCCESS;
}

int subsys_find(const SubsystemCollection *subsystems, const char *name)
{
    // Get the size of the collection
    int size = subsystems->size;

    // Iterate through each subsystem in the collection and compare if the subsystem's name is the same as the name provided by the user
    for (int i = 0; i < size; i++)
    {
        if (strcasecmp(subsystems->subsystems[i].name, name) == ERR_SUCCESS)
        {
            // Return the index of the subsytem if the name matches
            return i;
        }
    }

    return ERR_SYS_NOT_FOUND;
}

int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter)
{

    // Conditional check to see if the src, dest or filter is none
    if (src == NULL || dest == NULL || filter == NULL)
    {
        return ERR_NULL_POINTER;
    }

    // Created unsigned char values to hold the filter and wildcard mask
    unsigned char filter_mask = 0;
    unsigned char wildcard_mask = 0;

    // Use str len to get the filter length and set it to a variable
    int filter_length = strlen((const char *)filter);

    // Check whether or not the filter is empty
    if (filter_length != 8)
    {
        return ERR_INVALID_FILTER_STRING;
    }

    // Iterate through the filter array
    for (int i = 0; i < filter_length; i++)
    {
        // If the value at the index is a 1
        if (filter[i] == '1')
        {
            // Set the value at 7 - i to 1
            filter_mask |= (1 << (7 - i));
        }
        // If the value at the index is *
        else if (filter[i] == '*')
        {
            // Set the value at 7 -i in woldcard mask to 1
            wildcard_mask |= (1 << (7 - i));
        }
        // If the value does not equal 0, then we return invalif filter string
        else if (filter[i] != '0')
        {
            return ERR_INVALID_FILTER_STRING;
        }
    }

    // Flip the filter mask
    filter_mask = ~filter_mask;

    // Iterate though the src
    for (unsigned int i = 0; i < src->size; i++)
    {
        // Create a subsystem status variable set the the status field
        unsigned char subsystem_status = src->subsystems[i].status;

        // Set the result equal to the and operation between the filter mask xor the subsystem status, with an or of the wildcard mask
        unsigned char result = ((filter_mask) ^ (subsystem_status)) | (wildcard_mask);

        // If the result is equal to all 1's
        if (result == 0xFFu)
        {
            // Add the current subsystem to the destination subsystem
            dest->subsystems[dest->size++] = src->subsystems[i];
        }
    }

    return ERR_SUCCESS;
}

int subsys_filter_input(unsigned char *filter_string)
{
    // Print the prompt to the user asking for the filter_string
    printf("Enter filter string (8 characters of 1, 0, *): ");

    // Store the filter string in the variable
    scanf("%31s", filter_string);

    return ERR_SUCCESS;
}

int subsys_remove_input(char *subsys_name)
{
    // Print the prompt to the user asking for the name of the subsystem to remove
    printf("Enter subsystem name to remove: ");

    // Store the name of the subsytem to the variable
    scanf("%31s", subsys_name);

    return ERR_SUCCESS;
}

int add_subsystem_input(char *subsys_name)
{
    // Print the prompt to the user asking for the name of the subsystem to add
    printf("Enter subsystem name (no spaces): ");

    // Store the name of the subsystem to the variable
    scanf("%31s", subsys_name);

    return ERR_SUCCESS;
}
