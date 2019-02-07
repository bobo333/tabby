/*
    TODO:
    - parameterize file path
    - calculate padding size
    - don't print padding frame(s)
    - add frame header contents to frame struct
        - do all manipulation on the copied data to avoid seek errors
    - separate frame display function
    - general clean up
        - separate files
        - less inline printing
    - parse mpeg frames
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned long int length;
    unsigned char id[4];
    int has_flags;
} ID3Frame;

void print_hex(unsigned char *, int, int);
void print_chars(unsigned char *, int, int);
void print_binary(unsigned char *, int, int);
unsigned long int get_tag_size(unsigned char *, int);
void traverse_id3_frames(FILE *, unsigned long int, unsigned long int);
void get_frame_header_data(FILE *, ID3Frame *);
int in_padding(ID3Frame *);

int main() {
    unsigned char *header;
    unsigned long int tag_size;
    FILE *fptr;

    fptr = fopen("data/soviet_anthem.mp3", "rb");
    header = (unsigned char *)malloc(10 * sizeof(unsigned char));
    fread(header, 10, 1, fptr);

    print_hex(header, 0, 10);

    // identifier (3)
    printf("ID:\n");
    print_hex(header, 0, 3);
    print_chars(header, 0, 3);

    // version (2)
    printf("VERSION:\n");
    printf("Major: %d Minor: %d\n", header[3], header[4]);

    // flags (1)
    printf("FLAGS:\n");
    print_binary(header, 5, 1);

    // size (4)
    printf("SIZE:\n");
    print_hex(header, 6, 4);
    print_binary(header, 6, 4);
    tag_size = get_tag_size(header, 6);
    printf("tag size: %lu bytes\n", tag_size);

    // first frame header
    traverse_id3_frames(fptr, 10, tag_size);

    fclose(fptr);

    return 0;
}


void print_hex(unsigned char * data, int start, int num) {
    int i;

    for (i = 0; i < num; i++) {
        printf("%02hhx ", data[start + i]);
    }
    printf("\n");
}


void print_chars(unsigned char * data, int start, int num) {
    int i;

    for (i = 0; i < num; i++) {
        printf("%c", data[start + i]);
    }
    printf("\n");
}


void print_binary(unsigned char * data, int start, int num) {
    int i, j;
    char bits[9];

    for (i = 0; i < num; i++) {

        unsigned char byte = data[start + i];
        for (j = 0; j < 8; j++) {
            if (byte & 1) {
                bits[7-j] = '1';
            } else {
                bits[7-j] = '0';
            }
            byte>>=1;
        }
        bits[8] = '\0';

        printf("%s ", bits);
    }
    printf("\n");
}

unsigned long int get_tag_size(unsigned char * header, int size_start) {
    // start from last size byte, moving backwards
    // and tracking the exponent. The bytes are big-endian
    // note: this size represents the size WITHOUT the size of the header
    // itself

    unsigned long int total = 0;
    unsigned char byte;
    int i, j;
    int exponent = 0;

    print_binary(header, size_start, 4);

    for (i = size_start + 3; i >= size_start; i--) {
        byte = header[i];

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
