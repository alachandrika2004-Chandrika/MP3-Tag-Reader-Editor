#include <stdio.h>
#include <string.h>
#include "types.h"
#include "header.h"
#include "edit.h"

OperationType check_operation_type(char *argv[])
{
    if (argv[1] == NULL)
    {
        return e_unsupported;
    }

    if (strcmp(argv[1], "-r") == 0)
    {
        return e_read;
    }

    if (strcmp(argv[1], "-w") == 0)
    {
        return e_write;
    }

    return e_unsupported;
}

int main(int argc, char *argv[])
{
    Mp3 read;
    EditInfo edit;
    OperationType ret = check_operation_type(argv);

    if (ret == e_unsupported)
    {
        printf("Error! Invalid arguments\n");
        return 0;
    }

    if (ret == e_read)
    {
        if (read_validate_arguments(argv, &read) == e_failure)
        {
            printf("Invalid arguments for read\n");
            return 0;
        }

        printf("Read and validation completed\n");
        view_tag_details(&read);
    }

    if (ret == e_write)
    {
        if (read_and_validate_edit_args(argv, &edit) == e_failure)
        {
            printf("Invalid arguments for edit\n");
            return 0;
        }

        printf("Edit arguments validated\n");

        if (do_editing(&edit) == e_failure)
        {
            printf("Editing failed\n");
            return 0;
        }

        printf("Tag updated successfully\n");
    }

    return 0;
}