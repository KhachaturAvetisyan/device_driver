#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    FILE *file = fopen("./my_file.txt", "r");
   
    if (!file) {
        perror("Error opening file");
        return 1;
    }
   
    uint8_t buffer[16] = {0};
    size_t bytesRead = 0;
    size_t offset = 0;

    while ((bytesRead = fread(buffer, sizeof(uint8_t), (16 / sizeof(uint8_t)), file)) > 0) 
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

    fclose(file);
}
