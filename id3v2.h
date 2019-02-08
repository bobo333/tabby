#ifndef _id3v2_h
#define _id3v2_h

typedef struct {
    unsigned char id[3];
    unsigned char version[2];
    unsigned char flags;
    unsigned long int frames_size;
    unsigned long int total_size;
} ID3v2Header;

typedef struct {
    unsigned long int length;
    unsigned char id[4];
    int has_flags;
} ID3v2Frame;

ID3v2Header * parse_id3v2_header_data(unsigned char *);
void display_id3v2_header(ID3v2Header * header);

#endif
