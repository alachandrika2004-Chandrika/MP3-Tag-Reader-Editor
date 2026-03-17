#ifndef EDIT_H
#define EDIT_H

#include <stdio.h>
#include "types.h"

/* Structure to hold edit information for MP3 editing */
typedef struct _EditInfo
{
    char *audio_fname;   // Source MP3 filename
    char *tag;           // ID3 frame tag (e.g., "TIT2", "TPE1")
    char *new_data;      // Replacement text

    FILE *fptr_audio;    // Source MP3 file pointer
    FILE *fptr_temp;     // Temporary file pointer
} EditInfo;

/* Function prototypes */
Status read_and_validate_edit_args(char *argv[], EditInfo *editInfo);
Status do_editing(EditInfo *editInfo);
Status open_edit_files(EditInfo *editInfo);
Status copy_header(EditInfo *editInfo);
Status read_frame_header(char *frame_id, int *size, char *flags, EditInfo *editInfo);
Status edit_frames(EditInfo *editInfo);

#endif /* EDIT_H */