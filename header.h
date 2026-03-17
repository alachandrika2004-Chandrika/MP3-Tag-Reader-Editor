#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include "types.h"

/* Structure for MP3 file */
typedef struct _Mp3
{
    char *src_fname;
    FILE *fptr_mp3;

} Mp3;

/* Function declarations */

Status read_validate_arguments(char *argv[], Mp3 *mp3);
Status view_tag_details(Mp3 *mp3);

void skip_mp3_header(FILE *fptr);
Status read_tag(FILE *fptr, char *tag);
Status read_size(FILE *fptr, int *size);
Status read_tag_data(FILE *fptr, int size);

#endif