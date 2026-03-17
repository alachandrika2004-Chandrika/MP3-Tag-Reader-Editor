#include <stdio.h>
#include <string.h>
#include "types.h"
#include "header.h"

/* validate command line argument */
Status read_validate_arguments(char *argv[], Mp3 *mp3)
{
    if (argv[2] == NULL)
    {
        printf("MP3 file not provided\n");
        return e_failure;
    }

    if (strstr(argv[2], ".mp3") == NULL)
    {
        printf("Invalid MP3 file\n");
        return e_failure;
    }

    mp3->src_fname = argv[2];
    return e_success;
}

/* skip ID3 header */
void skip_mp3_header(FILE *fptr)
{
    fseek(fptr, 10, SEEK_SET);
}

/* read frame tag */
Status read_tag(FILE *fptr, char *tag)
{
    if (fread(tag, 1, 4, fptr) != 4)
        return e_failure;

    tag[4] = '\0';

    return e_success;
}

/* read frame size */
Status read_size(FILE *fptr, int *size)
{
    unsigned char arr[4];

    if (fread(arr, 1, 4, fptr) != 4)
        return e_failure;

    *size = (arr[0] << 24) |
            (arr[1] << 16) |
            (arr[2] << 8)  |
             arr[3];

    return e_success;
}

/* read normal text frames */
Status read_tag_data(FILE *fptr, int size)
{
    unsigned char encoding;

    fread(&encoding, 1, 1, fptr);
    size--;

    unsigned char data[1024];

    if (size > 1023)
        size = 1023;

    fread(data, 1, size, fptr);

    if (encoding == 1)   /* UTF-16 */
    {
        int i = 0;

        if ((data[0] == 0xFF && data[1] == 0xFE) ||
            (data[0] == 0xFE && data[1] == 0xFF))
        {
            i = 2;
        }

        for (; i < size; i += 2)
        {
            printf("%c", data[i]);
        }
    }
    else
    {
        data[size] = '\0';
        printf("%s", data);
    }

    printf("\n");

    return e_success;
}

/* view tag details */
Status view_tag_details(Mp3 *mp3)
{
    mp3->fptr_mp3 = fopen(mp3->src_fname, "rb");

    if (mp3->fptr_mp3 == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    skip_mp3_header(mp3->fptr_mp3);

    char tag[5];
    int size;

    char frames[6][5] = {"TIT2","TALB","TYER","TCON","COMM","TPE1"};
    char info[6][20]  = {"Title","Album","Year","Content Type","Comment","Artist"};

    for (int i = 0; i < 20; i++)
    {
        if (read_tag(mp3->fptr_mp3, tag) == e_failure)
            break;

        if (tag[0] == 0)
            break;

        read_size(mp3->fptr_mp3, &size);

        fseek(mp3->fptr_mp3, 2, SEEK_CUR);  /* skip flags */

        int found = 0;

        for (int j = 0; j < 6; j++)
        {
            if (strcmp(tag, frames[j]) == 0)
            {
                printf("\nDetails of TAG %s\n", frames[j]);
                printf("%s : ", info[j]);

                /* Special handling for COMM */
                if (strcmp(tag, "COMM") == 0)
                {
                    unsigned char buffer[1024];

                    if (size > 1023)
                        size = 1023;

                    fread(buffer, 1, size, mp3->fptr_mp3);

                    int index = 0;

                    index++;      /* skip encoding */
                    index += 3;   /* skip language */

                    /* skip description */
                    while (buffer[index] != '\0')
                    {
                        index++;
                    }

                    index++;

                    printf("%s\n", &buffer[index]);
                }
                else
                {
                    read_tag_data(mp3->fptr_mp3, size);
                }

                found = 1;
                break;
            }
        }

        if (!found)
        {
            fseek(mp3->fptr_mp3, size, SEEK_CUR);
        }
    }

    fclose(mp3->fptr_mp3);

    return e_success;
}