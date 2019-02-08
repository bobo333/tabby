#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

void check_file(char * file_name) {
    if (access(file_name, F_OK) != 0) {
        printf("Error: file %s not found\n", file_name);
        exit(1);
    }
    if (access(file_name, R_OK) != 0) {
        printf("Error: do not have read access to file %s\n", file_name);
        exit(1);
    }
}
