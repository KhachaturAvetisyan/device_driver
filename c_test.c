#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BYTES_PER_ROW   16U
#define OFFSET_SIZE     7U

void dec_to_hex(char *buff, size_t dec_num, uint8_t symbols_count)
{
    const char *table = "0123456789abcdef";

    for (uint8_t i = 1; i <= symbols_count; ++i)
    {
        buff[symbols_count - i] = table[dec_num % 16];
        dec_num /= 16;
    }
}

int main(void)
{

    const char *buff = "abcdabcdabcdabcd";
    size_t buff_size = strlen(buff);

    size_t rows = buff_size / BYTES_PER_ROW;
    size_t remaining = buff_size % BYTES_PER_ROW;

    size_t offset = 16;
    char *output_buff = malloc((rows + 2) * OFFSET_SIZE + (buff_size * 2) + );

    dec_to_hex(output_buff, offset, 7);

    printf("%.7s\n", output_buff);


//    for (size_t i = 0; i < rows; ++i)
//    {
//
//        for (size_t j = 0; j < BYTES_PER_ROW; ++j)
//        {
//
//        }
//    }

}