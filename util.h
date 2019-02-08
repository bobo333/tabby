#ifndef _util_h
#define _util_h

typedef enum {FALSE, TRUE} bool;

void print_chars(unsigned char *, int, int);
void print_hex(unsigned char *, int, int);
void print_binary(unsigned char *, int, int);
void check_file(char *);

#endif
