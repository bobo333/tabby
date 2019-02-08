/*
    TODO:
    - calculate padding size
    - add frame header contents to frame struct
        - do all manipulation on the copied data to avoid seek errors
    - general clean up
        - less inline printing
        - get tag size vs get frame size (combine?)
    - parse mpeg frames
    - tests
*/

#include <stdio.h>
#include <stdlib.h>

#include "id3v2.h"
#include "util.h"

void traverse_id3_frames(FILE *, unsigned long int, unsigned long int);
ID3v2Frame * get_frame_header_data(FILE *);
int in_padding(ID3v2Frame *);

int main(int argc, char *argv[]) {
    unsigned char *raw_header;
    char * file_name;
    ID3v2Header * header;
    FILE *fptr;

    if (argc < 2) {
        printf("Error: please provide path to file\n");
        exit(1);
    }

    // check file exists and can be read
    file_name = argv[1];
    check_file(file_name);
    fptr = fopen(file_name, "rb");

    raw_header = (unsigned char *)malloc(10 * sizeof(unsigned char));
    fread(raw_header, 10, 1, fptr);
    header = parse_id3v2_header(raw_header);
    free(raw_header);
    display_id3v2_header(header);

    traverse_id3_frames(fptr, 10, header->total_size);

    fclose(fptr);

    return 0;
}

void traverse_id3_frames(FILE * f, unsigned long int pos, unsigned long int length) {
    ID3v2Frame * frame;

    while (pos < length) {
        frame = get_frame_header_data(f);
        pos += 10;
        pos += frame->content_size;
        fseek(f, frame->content_size, SEEK_CUR);

        // check if we've hit padding
        if (in_padding(frame) == 1) {
            printf("end of id3v2 frames\n");
            break;
        }

        display_id3v2_frame(frame);
    }

    free(frame);
}

ID3v2Frame * get_frame_header_data(FILE * f) {
    unsigned char * header = (unsigned char *)malloc(10 * sizeof(unsigned char));
    fread(header, 10, 1, f);

    return parse_id3v2_frame(header);
}

int in_padding(ID3v2Frame * frame) {
    int i;
    for (i = 0; i < 4; i++) {
        if (frame->id[i] != 0) {
            return 0;
        }
    }

    return 1;
}
