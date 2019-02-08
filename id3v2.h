#ifndef _id3v2_h
#define _id3v2_h

#include <stdio.h>

typedef struct {
    unsigned char id[3];
    unsigned char version[2];
    unsigned char flags;
    unsigned long int frames_size;
    unsigned long int total_size;
} ID3v2Header;

void parse_id3v2_header(FILE *, ID3v2Header *);
void display_id3v2_header(ID3v2Header *);

void traverse_id3v2_frames(FILE *, unsigned long int, unsigned long int);

#endif
