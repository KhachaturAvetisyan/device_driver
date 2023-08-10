#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    FILE *input_file = fopen("./my_file.txt", "r");
   
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen("/tmp/output", "w");
    
    if (!output_file) {
        perror("Error opening output file");
        return 1;
    }
   
    uint8_t buffer[16] = {0};
    size_t bytesRead = 0;
    size_t offset = 0;

    while ((bytesRead = fread(buffer, sizeof(uint8_t), (16 / sizeof(uint8_t)), input_file)) > 0) 
    {
        printf("%07x", offset);

        for (size_t i = 0; i < bytesRead; i += 2)
        {
            printf(" %02x%02x", buffer[i+1], buffer[i]);
        }

        printf("\n");

        offset += bytesRead;
        memset(buffer, 0, 16);
    }

    printf("%07x\n", offset);

    fclose(input_file);
}
