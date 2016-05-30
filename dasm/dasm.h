#ifndef _DASM_
#define _DASM_
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/elf_reader.h"


enum op31_26
{
    SPECIAL = 0,
    ADDI = 8
};

enum op5_0
{
    ADD = 32
};
char *name31_26[] = {
    "special",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "ADDI"

};

char *name5_0[] = {"","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","ADD"
};


void dasm_line(uint8_t *bytes, size_t text_size);

uint8_t get_bits_31_26(uint32_t word);
uint8_t get_bits_25_21(uint32_t word);
uint8_t get_bits_20_16(uint32_t word);
uint8_t get_bits_15_11(uint32_t word);
uint8_t get_bits_10_6(uint32_t word);
uint8_t get_bits_5_0(uint32_t word);
uint32_t get_bits_15_0(uint32_t word);




#endif /* end of include guard: _DASM_ */
