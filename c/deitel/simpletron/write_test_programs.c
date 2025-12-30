#include <stdint.h>
#include <stdio.h>
#include "simpletron.h"

void write_bindata(void) {
    FILE *file = fopen("add_numbers.sm", "wb");
    const int32_t header = 0b11110101;
    word_t data[] = {0x100a, 0x100b, 0x200a, 0x300b, 0x210c, 0x110c, 0x4300};
    fwrite(&header, sizeof(int32_t), 1, file);
    for (size_t i = 0; i < sizeof(data) / sizeof(word_t); i++)
        fwrite(&data[i], sizeof(word_t), 1, file);
    fclose(file);
}

void write_data(void) {
    /* simpletron machine language */
    FILE *file = fopen("add_numbers.sml", "w");
    char data[][WORD_BYTES * 2 + 1] = {"100a", "100b", "200a", "300b", "210c", "110c", "4300"};
    for (size_t i = 0; i < sizeof(data) / sizeof(char) / (WORD_BYTES * 2 + 1); i++)
        fprintf(file, "%s\n", data[i]);
    fclose(file);
}

int main() {
    write_bindata();
    write_data();
    return 0;
}