#include "dasm.h"

int main(int argc, char const *argv[]) {
    struct elf_descr *elf;

    uint32_t text_addr = 0;
    uint8_t *text_bytes = NULL;
    size_t text_size = 0;
    uint8_t text_align = 0;


    elf = read_elf(argv[1]);


    get_text_section(elf, &text_bytes, &text_size, &text_addr, &text_align);

    dasm_line(text_bytes, text_size);
    return 0;
}

void dasm_line(uint8_t *bytes, size_t text_size)
{
    uint32_t word;
    for (uint32_t i = 0; i < text_size; i+=4) {
        word = bytes[i+3] + (bytes[i+2]<<8) + (bytes[i+1]<<16) + (bytes[i]<<24);
        //printf("%u %u %u %u %u %u\n", get_bits_31_26(word), get_bits_25_21(word), get_bits_20_16(word), get_bits_15_11(word), get_bits_10_6(word), get_bits_5_0(word));
        if (!get_bits_31_26(word))
        {
            printf("%s $%u, $%u, $%u\n", name5_0[get_bits_5_0(word)], get_bits_15_11(word), get_bits_25_21(word), get_bits_20_16(word));
        } else {
            printf("%s $%u, $%u, %u\n", name31_26[get_bits_31_26(word)], get_bits_20_16(word), get_bits_25_21(word), get_bits_15_0(word));
        }
        printf("%u\n", get_bits(word, 20, 16));
    }
}

uint8_t get_bits_31_26(uint32_t word)
{
    return (word & 0xFC000000)>>26;
}

uint8_t get_bits_25_21(uint32_t word)
{
    return (word & 0x3E00000)>>21;
}

uint8_t get_bits_20_16(uint32_t word)
{
    return (word & 0x1F0000)>>16;
}

uint8_t get_bits_15_11(uint32_t word)
{
    return (word & 0xF800)>>11;
}

uint8_t get_bits_10_6(uint32_t word)
{
    return (word & 0x7C0)>>6;
}

uint8_t get_bits_5_0(uint32_t word)
{
    return (word & 0x3F);
}

uint32_t get_bits_15_0(uint32_t word)
{
    return (word & 0xFFFF);
}

uint32_t get_bits(uint32_t word, uint8_t x, uint8_t y)
{
    return (word << (31-x) >> y); // marche pas
}
