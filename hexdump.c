
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

void dump_hex (FILE *fp);

// The number of bytes to display on a line
uint32_t LINE_WIDTH = 16;

FILE *fp;

int
main (int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./hexdump <file>\n");
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        //fprintf(stderr, "Error: Unable to open file.\n");
        perror("ERROR | Unable to open file");
    }

    dump_hex(fp);

    fclose(fp);
}

void
dump_hex (FILE *fp) {
    uint8_t *line;              // Contains the bytes to be displayed
    size_t index = 0;           // Index to the bytes in 'line'

    uint32_t line_number = 0;   // Indicates the index of the first byte in the line
    uint8_t byte;               // A temporary placeholder to read 1 byte into

    line = malloc(sizeof(uint8_t) * LINE_WIDTH);

    // While a byte has been read, process it.
    while (fread(&byte, 1, 1, fp)) {
        line[index++] = byte;

        if (index >= LINE_WIDTH) {
            printf("0x%010d | ", line_number);
            line_number += LINE_WIDTH;

            for (int i = 0; i < LINE_WIDTH; i++) {
                printf("%02X ", line[i]);
            }

            printf("|");
            for (int i = 0; i < LINE_WIDTH; i++) {
                char c = (char) line[i];

                if (isalpha(c) || isdigit(c) || ispunct(c)) {
                    printf("%c", c);
                }
                else {
                    printf(".");
                }
            }
            printf("|\n");

            index = 0;
        }
    }

    // print the remaining bytes that were cut off
    if (index > 0) {
        printf("0x%010d | ", line_number);

        for (int i = 0; i < index; i++) {
            printf("%02X ", line[i]);
        }

        int filler = LINE_WIDTH - index;

        for (int i = 0 ; i < filler; i++) {
            printf("   ");
        }

        printf("|");
        for (int i = 0; i < index; i++) {
            char c = (char) line[i];

            if (isalpha(c) || isdigit(c) || ispunct(c)) {
                printf("%c", c);
            }
            else {
                printf(".");
            }
        }

        for (int i = 0; i < filler; i++) {
            printf(" ");
        }
        printf("|\n");
    }

    free(line);
}
