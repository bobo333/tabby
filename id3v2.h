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

typedef struct {
    unsigned long int content_size;
    unsigned long int total_size;
    unsigned char id[4];
    unsigned char flags[2];
    int has_flags;
} ID3v2Frame;

ID3v2Header * parse_id3v2_header(unsigned char *);
void display_id3v2_header(ID3v2Header *);

ID3v2Frame * parse_id3v2_frame(unsigned char *);
void display_id3v2_frame(ID3v2Frame *);
void traverse_id3_frames(FILE *, unsigned long int, unsigned long int);

#endif
