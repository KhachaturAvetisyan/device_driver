#include <stdio.h>
#include <stdlib.h>

#define BYTES_PER_ROW 16

void print_hex_ascii(const unsigned char *data, size_t size) {
    size_t i;

    for (i = 0; i < size; i++) {
        if (i % BYTES_PER_ROW == 0) {
            printf("%08zX ", i);
        }

        printf("%02X ", data[i]);

        if (i % BYTES_PER_ROW == BYTES_PER_ROW - 1) {
            printf(" | ");
            for (size_t j = i - BYTES_PER_ROW + 1; j <= i; j++) {
                if (data[j] >= 32 && data[j] <= 126) {
                    printf("%c", data[j]);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }

    // If the last row is not complete, add extra spaces
    size_t remaining = BYTES_PER_ROW - (i % BYTES_PER_ROW);
    if (remaining != BYTES_PER_ROW) {
        for (size_t j = 0; j < remaining; j++) {
            printf("   ");
        }
        printf(" | ");
        for (size_t j = i - (i % BYTES_PER_ROW); j < i; j++) {
            if (data[j] >= 32 && data[j] <= 126) {
                printf("%c", data[j]);
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    unsigned char buffer[BYTES_PER_ROW];
    size_t bytesRead;

    do {
        bytesRead = fread(buffer, 1, BYTES_PER_ROW, file);
        if (bytesRead > 0) {
            print_hex_ascii(buffer, bytesRead);
        }
    } while (bytesRead == BYTES_PER_ROW);

    fclose(file);
    return 0;
}

