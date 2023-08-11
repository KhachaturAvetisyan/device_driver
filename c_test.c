#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BYTES_PER_ROW       16U
#define OFFSET_SIZE         7U
#define SYMBOLS_PER_ROW     (OFFSET_SIZE + 1 + BYTES_PER_ROW / 2 * 5)


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
    const char *buff = "abcdabcdabcdabcd\n";
    size_t buff_size = strlen(buff);

    size_t rows = (buff_size + BYTES_PER_ROW - 1) / BYTES_PER_ROW;

    size_t offset = 0;
    size_t output_buff_size = rows * SYMBOLS_PER_ROW + OFFSET_SIZE + 1;

    printf("%ld %ld\n", buff_size, output_buff_size);

    char *output_buff = malloc(output_buff_size);

    uintptr_t buf_iter = 0;
    size_t i, j;
    for (i = 0; i < rows; ++i)
    {
        dec_to_hex(&output_buff[buf_iter], offset, OFFSET_SIZE);
        buf_iter += OFFSET_SIZE;

        output_buff[buf_iter++] = ' ';

        j = 0;
        while (j < BYTES_PER_ROW)
        {
            if (i * BYTES_PER_ROW + j > buff_size)
            {
//                buf_iter +=
                break;
            }
            else
            {
                dec_to_hex(&output_buff[buf_iter + 2], buff[i * BYTES_PER_ROW + j], 2);
                dec_to_hex(&output_buff[buf_iter], buff[i * BYTES_PER_ROW + j + 1], 2);
                buf_iter += 4;
                output_buff[buf_iter++] = j != BYTES_PER_ROW - 2 ? ' ' : '\n';
            }
            j += 2;
        }
        offset += j;
    }
    dec_to_hex(&output_buff[buf_iter], offset, OFFSET_SIZE);
    buf_iter += OFFSET_SIZE;
    output_buff[buf_iter++] = '\n';

    printf("%.104s\n", output_buff);
}