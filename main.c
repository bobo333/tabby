/*
    TODO:
    - calculate padding size
    - don't print padding frame(s)
    - add frame header contents to frame struct
        - do all manipulation on the copied data to avoid seek errors
    - separate frame display function
    - general clean up
        - less inline printing
        - get tag size vs get frame size (combine?)
    - parse mpeg frames
    - tests
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "id3v2.h"
#include "util.h"

typedef struct {
    unsigned long int length;
    unsigned char id[4];
    int has_flags;
} ID3Frame;

unsigned long int get_tag_size(unsigned char *, int);
void traverse_id3_frames(FILE *, unsigned long int, unsigned long int);
void get_frame_header_data(FILE *, ID3Frame *);
int in_padding(ID3Frame *);

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

    header = parse_id3v2_header_data(raw_header);
    display_id3v2_header(header);

    free(raw_header);

    // // first frame header
    // traverse_id3_frames(fptr, 10, tag_size);

    fclose(fptr);

    return 0;
}

unsigned long int get_frame_size(unsigned char * header, int size_start) {
    // for id3v2.3.0, the frame size is NOT synch-safe
    // note: this size represents the size WITHOUT the size of the header
    // itself

    unsigned long int total = 0;
    unsigned char byte;
    int i, j;
    int exponent = 0;

    print_binary(header, size_start, 4);

    for (i = size_start + 3; i >= size_start; i--) {
        byte = header[i];

        // use the last bit, NOT "synch-safe integers"
        for (j = 0; j < 8; j++, exponent++) {
            if (byte & 1) {
                total += (unsigned long int)1 << exponent;    // exponent of 2
            }
            byte >>= 1;
        }
    }

    return total;
}

void traverse_id3_frames(FILE * f, unsigned long int pos, unsigned long int length) {
    ID3Frame * frame = malloc(sizeof(ID3Frame));

    while (pos < length) {
        get_frame_header_data(f, frame);
        pos += 10;
        pos += frame->length;
        fseek(f, frame->length, SEEK_CUR);
        print_chars(frame->id, 0, 4);

        // check if we've hit padding
        if (in_padding(frame) == 1) {
            printf("end of id3v2 frames\n");
            break;
        }

        printf("length: %lu\n", frame->length);
        printf("pos: %lu, len: %lu\n", pos, length);
    }

    free(frame);
}

void get_frame_header_data(FILE * f, ID3Frame * frame) {
    unsigned char * header = (unsigned char *)malloc(10 * sizeof(unsigned char));
    unsigned long int frame_size;

    fread(header, 10, 1, f);
    printf("\nframe:\n");
    printf("header: ");
    print_hex(header, 0, 10);
    printf("frame id: ");
    print_chars(header, 0, 4);
    frame_size = get_frame_size(header, 4);
    printf("frame size: %lu bytes\n", frame_size);
    printf("flags: ");
    print_binary(header, 8, 2);

    frame->length = frame_size;
    memcpy(frame->id, header, 4);
}

int in_padding(ID3Frame * frame) {
    int i;
    for (i = 0; i < 4; i++) {
        if (frame->id[i] != 0) {
            return 0;
        }
    }

    return 1;
}
