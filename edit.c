#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "edit.h"

char* tag_by_type(char *option)
{
    if (strcmp(option, "-a") == 0)
        return "TPE1";   
    else if (strcmp(option, "-t") == 0)
        return "TIT2";   
    else if (strcmp(option, "-c") == 0)
        return "TCON";   
    else if (strcmp(option, "-y") == 0)
        return "TYER"; 
    else if (strcmp(option, "-m") == 0)
        return "COMM";   
    else if (strcmp(option, "-A") == 0)
        return "TALB";   
    else
        return NULL;
}

Status read_and_validate_edit_args(char *argv[], EditInfo *editInfo)
{
    if (argv[2] == NULL)
    {
        printf("ERROR: Tag option not passed\n");
        return e_failure;
    }

    char *tag = tag_by_type(argv[2]);

    if (tag == NULL)
    {
        printf("ERROR: Invalid tag option\n");
        return e_failure;
    }

    editInfo->tag = tag;

    if (argv[3] == NULL)
    {
        printf("ERROR: New data not passed\n");
        return e_failure;
    }

    editInfo->new_data = argv[3];

    if (argv[4] == NULL)
    {
        printf("ERROR: MP3 file not passed\n");
        return e_failure;
    }

    if (strstr(argv[4], ".mp3") == NULL)
    {
        printf("ERROR: Invalid audio file\n");
        return e_failure;
    }

    editInfo->audio_fname = argv[4];

    return e_success;
}

Status do_editing(EditInfo *editInfo)
{
    Status ret;

    ret = open_edit_files(editInfo);
    if (ret == e_failure)
        return e_failure;

    ret = copy_header(editInfo);
    if (ret == e_failure)
        return e_failure;

    ret = edit_frames(editInfo);
    if (ret == e_failure)
        return e_failure;

    char ch;
    while (fread(&ch, 1, 1, editInfo->fptr_audio) == 1)
    {
        fwrite(&ch, 1, 1, editInfo->fptr_temp);
    }

    fclose(editInfo->fptr_audio);
    fclose(editInfo->fptr_temp);

    remove(editInfo->audio_fname);
    rename("temp.mp3", editInfo->audio_fname);

    return e_success;
}

Status open_edit_files(EditInfo *editInfo)
{
    editInfo->fptr_audio = fopen(editInfo->audio_fname, "rb");
    if(editInfo->fptr_audio == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", editInfo->audio_fname);
        return e_failure;
    }

    editInfo->fptr_temp = fopen("temp.mp3", "wb");
    if (editInfo->fptr_temp == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open temp file\n");
        return e_failure;
    }

    return e_success;
}

Status copy_header(EditInfo *editInfo)
{
    char header[10];
    if (fread(header, 10, 1, editInfo->fptr_audio) != 1)
    {
        printf("ERROR: Unable to read header\n");
        return e_failure;
    }

    if (fwrite(header, 10, 1, editInfo->fptr_temp) != 1)
    {
        printf("ERROR: Unable to write header\n");
        return e_failure;
    }

    return e_success;
}

Status read_frame_header(char *frame_id, int *size, char *flags, EditInfo *editInfo)
{
    if (fread(frame_id, 4, 1, editInfo->fptr_audio) != 1)
        return e_failure;

    frame_id[4] = '\0';
    unsigned char size_buffer[4];

    if (fread(size_buffer, 4, 1, editInfo->fptr_audio) != 1)
        return e_failure;

    *size = (size_buffer[0] << 24) | (size_buffer[1] << 16) | (size_buffer[2] << 8)  | (size_buffer[3]);

    if (fread(flags, 2, 1, editInfo->fptr_audio) != 1)
        return e_failure;

    return e_success;
}

void int_to_bigendian(int size, unsigned char *buffer)
{
    buffer[0] = (size >> 24) & 0xFF;
    buffer[1] = (size >> 16) & 0xFF;
    buffer[2] = (size >> 8) & 0xFF;
    buffer[3] = size & 0xFF;
}

Status edit_frames(EditInfo *editInfo)
{
    int tag_found = 0;
    while (1)
    {
        char frame_id[5] = {0};
        char flags[2];
        int size;

        if (read_frame_header(frame_id, &size, flags, editInfo) == e_failure)
        {
            return e_failure; 
        }

        if (frame_id[0] == 0) 
        {
            break;
        }

        if (strcmp(frame_id, editInfo->tag) == 0)
        {
            tag_found = 1;

            if (fwrite(frame_id, 4, 1, editInfo->fptr_temp) != 1)
                return e_failure;

            printf("New data to edit tag %s : %s\n",frame_id,editInfo->new_data);
            int new_size = strlen(editInfo->new_data) + 1;
            printf("New data size %d\n",new_size);
            unsigned char buffer[4];
            int_to_bigendian(new_size, buffer);

            fwrite(buffer, 4, 1, editInfo->fptr_temp);
            fwrite(flags, 2, 1, editInfo->fptr_temp);

            unsigned char encoding = 0x00;
            fwrite(&encoding, 1, 1, editInfo->fptr_temp);
            fwrite(editInfo->new_data, strlen(editInfo->new_data), 1, editInfo->fptr_temp);

            fseek(editInfo->fptr_audio, size, SEEK_CUR);
        }
        
        else
        {
            if (fwrite(frame_id, 4, 1, editInfo->fptr_temp) != 1)
                return e_failure;

            unsigned char buffer[4];
            int_to_bigendian(size, buffer);

            if (fwrite(buffer, 4, 1, editInfo->fptr_temp) != 1)
                return e_failure;

            if (fwrite(flags, 2, 1, editInfo->fptr_temp) != 1)
                return e_failure;

            char *data = malloc(size);
            if (data == NULL)
                return e_failure;

            if (fread(data, size, 1, editInfo->fptr_audio) != 1)
            {
                free(data);
                return e_failure;
            }

            if (fwrite(data, size, 1, editInfo->fptr_temp) != 1)
            {
                free(data);
                return e_failure;
            }
            free(data);
        }
    }
    if (!tag_found)
    {
        printf("Tag not found\n");
        return e_failure;
    }
    return e_success;
}