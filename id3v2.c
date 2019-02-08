#include "id3v2.h"
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

unsigned long int get_frames_size(unsigned char *, int);
unsigned long int get_frame_size(unsigned char *, int);

/*
    Header / Full Tag
*/

ID3v2Header * parse_id3v2_header(unsigned char * raw_header) {
    int i, j;
    ID3v2Header * header = malloc(sizeof(ID3v2Header));

    // bytes 0-2 are ID
    for (i = 0; i < 3; i++) {
        header->id[i] = raw_header[i];
    }
    // bytes 3-4 are Version
    for (i = 3, j = 0; i < 5; i++, j++) {
        header->version[j] = raw_header[i];
    }
    // byte 5 is flags
    header->flags = raw_header[5];
    // bytes 6-9 are size
    header->frames_size = get_frames_size(raw_header, 6);
    header->total_size = header->frames_size + 10;

    return header;
}

void display_id3v2_header(ID3v2Header * header) {
    printf("id: ");
    print_chars(header->id, 0, 3);
    printf("version: ID3v2.%d.%d\n", header->version[0], header->version[1]);
    printf("flags: ");
    print_binary(&header->flags, 0, 1); // sort of a hack to avoid making an array?
    printf("frames size: %lu bytes\n", header->frames_size);
    printf("total  size: %lu bytes\n", header->total_size);
}

unsigned long int get_frames_size(unsigned char * raw_header, int size_start) {
    // start from last size byte, moving backwards
    // and tracking the exponent. The bytes are big-endian
    // note: this size represents the size WITHOUT the size of the header itself

    unsigned long int total = 0;
    unsigned char byte;
    int i, j;
    int exponent = 0;

    for (i = size_start + 3; i >= size_start; i--) {
        byte = raw_header[i];

        // ignore the last bit, (it's always set to 0), "synch-safe integers"
        for (j = 0; j < 7; j++, exponent++) {
            if (byte & 1) {
                total += (unsigned long int)1 << exponent;    // exponent of 2
            }
            byte >>= 1;
        }
    }

    return total;
}


/*
    Frames
*/

ID3v2Frame * parse_id3v2_frame(unsigned char * frame_header) {
    ID3v2Frame * frame = malloc(sizeof(ID3v2Frame));
    int i, j;

    // bytes 0-3 are ID
    for (i = 0; i < 4; i++) {
        frame->id[i] = frame_header[i];
    }
    // bytes 4-7 are size
    frame->content_size = get_frame_size(frame_header, 4);
    // bytes 8-9 are flags
    for (i = 8, j = 0; i < 10; i++,j++) {
        frame->flags[j] = frame_header[i];
    }

    return frame;
}

void display_id3v2_frame(ID3v2Frame * frame) {
    printf("id: ");
    print_chars(frame->id, 0, 4);
    printf("content_size: %lu\n", frame->content_size);
}

unsigned long int get_frame_size(unsigned char * header, int size_start) {
    // for id3v2.3.0, the frame size is NOT synch-safe ints
    // note: this size represents the size WITHOUT the size of the header
    // itself

    unsigned long int total = 0;
    unsigned char byte;
    int i, j;
    int exponent = 0;

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

// traverse id3v2 frames
