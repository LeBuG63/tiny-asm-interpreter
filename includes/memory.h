#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stdbool.h>

#define N_REGISTERS     7 // 7 ne doit pas etre utilise
#define MEM_SIZE        0x8000
#define N_FUNCTIONS     7

enum errorcode_t {
    UNKNOWN,
    SUCCESS,
    MISSING_RIGHTBRACKET,
    OUTOFBOUNDS,
    NOLABEL
};

struct token_t {
    char    insname[8];

    char    dest[8];
    char    src[8];
};

enum errorcode_t    set_mem(uint32_t addr, uint32_t value);
enum errorcode_t    get_value_memory(int16_t *val, uint32_t addr);
uint32_t            get_index_reg(const char *regname);

struct token_t      *read_from_memcode(uint32_t src);
enum errorcode_t    get_regdest_val(uint32_t *addr, int16_t *val, char *src, char *dest);

enum errorcode_t    parse_arg_string(const char *str, uint32_t *addr, int16_t *val);

void                set_memorycode(const char *line, uint32_t index);

#endif // __MEMORY_H__