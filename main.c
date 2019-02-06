#include <stdio.h>
#include <stdlib.h>

FILE *fptr;

void print_hex(unsigned char *, int, int);
void print_chars(unsigned char *, int, int);
void print_binary(unsigned char *, int, int);

int main() {
    unsigned char *header;

    fptr = fopen("data/soviet_anthem.mp3", "rb");
    header = (unsigned char *)malloc(10 * sizeof(unsigned char));
    fread(header, 10, 1, fptr);
    fclose(fptr);

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
