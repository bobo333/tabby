/*
    TODO:
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
