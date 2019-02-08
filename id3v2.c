#include "id3v2.h"
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

unsigned long int get_tag_size(unsigned char * raw_header, int size_start);

ID3v2Header * parse_id3v2_header_data(unsigned char * raw_header) {
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
    header->frames_size = get_tag_size(raw_header, 6);
    header->total_size = header->frames_size + 10;

    return header;
}

void display_id3v2_header(ID3v2Header * header) {
    printf("id: %c%c%c\n", header->id[0], header->id[1], header->id[2]);
    printf("version: ID3v2.%d.%d\n", header->version[0], header->version[1]);
    printf("flags: ");
    print_binary(&header->flags, 0, 1); // sort of a hack to avoid making an array?
    printf("frames size: %lu bytes\n", header->frames_size);
    printf("total  size: %lu bytes\n", header->total_size);
}

unsigned long int get_tag_size(unsigned char * raw_header, int size_start) {
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


// get id3v2 header data

// traverse id3v2 frames

// display id3v2 frame
