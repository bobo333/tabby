#include "util.h"
#include <stdio.h>

void print_chars(unsigned char * data, int start, int num) {
    int i;

    for (i = 0; i < num; i++) {
        printf("%c", data[start + i]);
    }
    printf("\n");
}

void print_hex(unsigned char * data, int start, int num) {
    int i;

    for (i = 0; i < num; i++) {
        printf("%02hhx ", data[start + i]);
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
