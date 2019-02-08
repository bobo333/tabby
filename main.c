/*
    TODO:
    - add frame header contents to frame struct
        - do all manipulation on the copied data to avoid seek errors
    - general clean up
        - get tag size vs get frame size (combine?)
    - parse mpeg frames
    - tests
*/

#include <stdlib.h>

#include "id3v2.h"
#include "util.h"

int main(int argc, char *argv[]) {
    char * file_name;
    FILE *fptr;
    ID3v2Header * header = malloc(sizeof(ID3v2Header));

    if (argc < 2) {
        printf("Error: please provide path to file\n");
        exit(1);
    }

    // check file exists and can be read
    file_name = argv[1];
    check_file(file_name);
    fptr = fopen(file_name, "rb");

    parse_id3v2_header(fptr, header);
    display_id3v2_header(header);
    traverse_id3v2_frames(fptr, 10, header->total_size);

    free(header);
    fclose(fptr);

    return 0;
}
