#include "subsystem.h"
#include <string.h>

int subsys_init(Subsystem *subsystem, const char *name, char status)
{
    // Set/Initialize the attributes of the subsystem
    subsystem->status = status;
    subsystem->data = 0;

    // Copy the name into the name field of the subsystem, and ensure it's null terminated
    strncpy(subsystem->name, name, MAX_STR);
    subsystem->name[MAX_STR - 1] = '\0';

    return ERR_SUCCESS;
}

int subsys_print(Subsystem *subsystem)
{
    // Check if the subsystem is not null
    if (subsystem != NULL)
    {
        // Create an unsigned integer to hold the new data
        unsigned int new_data;

        // Print the name and the status
        printf("Name: %16s, Status: ", subsystem->name);

        // Print the status of the subsystem
        subsys_status_print(subsystem);

        // If we successfully get the data if the subsystem
        if (subsys_data_get(subsystem, &new_data) == ERR_SUCCESS)
        {
            printf(", Data: 0x%08X", new_data);
        }

        // Print a new line and indicate success by returning ERR_SUCCESS
        printf("\n");
        return ERR_SUCCESS;
    }

    return ERR_NULL_POINTER;
}

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value)
{
    // Check if there is a null subsystem, if so return null pointer error
    if (subsystem == NULL)
    {
        return ERR_NULL_POINTER;
    }

    // Assign current status to the subsystems status
    unsigned char current_status = subsystem->status;

    // Set the mask to the result of the left shift operator of status onto value
    unsigned char mask = value << status;

    // If status is a 2 bit value
    if ((status == STATUS_RESOURCE || status == STATUS_PERFORMANCE))
    {
        // Check whether or not value is less then 4
        if (value < 4)
        {
            // Clear the bits first before setting them
            current_status = current_status & ~(0x03u << status);
        }
        else
        {
            return ERR_INVALID_VALUE;
        }
    }
    // Else, if there's any other status
    else if (status == STATUS_ERROR || status == STATUS_POWER || status == STATUS_DATA || status == STATUS_ACTIVITY)
    {
        // If value is greater then 2
        if (value < 2)
        {
            // Clear the bits first before setting them
            current_status = current_status & ~(1 << status);
        }
        else
        {
            // Else, return a invalid value error
            return ERR_INVALID_VALUE;
        }
    }
    else
    {
        // Else return an invalud status error
        return ERR_INVALID_STATUS;
    }

    // Set the bits
    current_status = mask | current_status;
    subsystem->status = current_status;

    return ERR_SUCCESS;
}

int subsys_status_print(const Subsystem *subsystem)
{
    // Intialize neccessary variables
    int status;
    int index = 0;
    char status_binary[9];

    // Add null terminator
    status_binary[8] = '\0';

    // If subsystem is not null
    if (subsystem != NULL)
    {
        // Set the status to the status field of the subsystem
        status = subsystem->status;

        // create a while loop which will loop until we reach 8
        while (index < 8)
        {
            // Converting to binary
            status_binary[index] = (status % 2) + '0';
            status = status / 2;

            // Increment index
            index++;
        }

        // Initialize performance and status to be the conversion of their corresponding bits
        int performance = (status_binary[STATUS_PERFORMANCE] - '0') + 2 * (status_binary[STATUS_PERFORMANCE + 1] - '0');
        int status = (status_binary[STATUS_RESOURCE] - '0') + 2 * (status_binary[STATUS_RESOURCE + 1] - '0');

        // Print the status of the subsystem, with the result from each status field
        printf("(PWR: %c|DATA: %c|ACT: %c|ERR: %c|PERF: %d|RES: %d)",
               status_binary[STATUS_POWER], status_binary[STATUS_DATA], status_binary[STATUS_ACTIVITY], status_binary[STATUS_ERROR],
               performance, status);

        return ERR_SUCCESS;
    }

    return ERR_NULL_POINTER;
}

int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data)
{
    // If the old data being sent in, or the subsystem is null, return ERR_NULL_POINTER
    if (old_data == NULL || subsystem == NULL)
    {
        return ERR_NULL_POINTER;
    }

    // Set the value of the old data pointer to the data field of the subsystem
    *old_data = subsystem->data;

    // Set the subsystem data field to the new data
    subsystem->data = new_data;

    // Call the subsystem status set in order to set the status
    subsys_status_set(subsystem, STATUS_DATA, 1);

    return ERR_SUCCESS;
}

int subsys_data_get(Subsystem *subsystem, unsigned int *data)
{
    // Check whether or not the subsystem is null
    if (subsystem == NULL)
    {
        return ERR_NULL_POINTER;
    }

    // Check if the status data bit is 0
    if ((subsystem->status & (1 << STATUS_DATA)) == 0)
    {
        // Set the data to 0
        *data = 0;

        // Return no data error
        return ERR_NO_DATA;
    }

    // If not, set the data to the subsystem data field
    *data = subsystem->data;

    // Set the subsystem data field to 0
    subsystem->data = 0;

    // Clear the status data bit
    subsystem->status = subsystem->status & ~(1 << STATUS_DATA);

    return ERR_SUCCESS;
}

int subsys_data_set_input(char *subsys_name, unsigned int *data)
{
    // Print out the required information
    printf("Enter <Subsystem Name> <Data, uppercase hex without 0x>: ");

    // Use scanf to get the informaiton name as a stirng and data as a ahex
    scanf("%31s %x", subsys_name, data);

    return ERR_SUCCESS;
}

int print_subsystem_input(char *subsys_name)
{
    // Ask the user for the name of the subsystem
    printf("Enter the name of the subsystem: ");

    // Use scanf to get the name
    scanf("%31s", subsys_name);

    return ERR_SUCCESS;
}

int subsys_status_set_input(char *subsys_name, unsigned char *status, unsigned char *value)
{
    // Print out the desired format of the information to the user
    printf("Enter <Subsystem Name> <Status ID: 7, 6, 5, 4, 2, or 0> <New Value: [0 -> 3]>: ");

    // Use scanf to get the required information
    scanf("%31s %hhu %hhu", subsys_name, status, value);

    return ERR_SUCCESS;
}
