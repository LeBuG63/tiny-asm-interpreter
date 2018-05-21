#include "memory.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int16_t          memory[MEM_SIZE] = {0};
static char             memorycode[MEM_SIZE][128];
static struct pcstack_t pcstack;

enum errorcode_t set_mem(uint32_t addr, uint32_t value) {
    if(addr > MEM_SIZE)
        return OUTOFBOUNDS;

    memory[addr] = value;

    return SUCCESS;
}

enum errorcode_t  get_value_memory(int16_t *val, uint32_t addr) {
    if(addr > MEM_SIZE)
        return OUTOFBOUNDS;
    
    *val = memory[addr];

    return SUCCESS;
}

struct token_t *read_from_memcode(uint32_t src) {
    if(src >= MEM_SIZE) 
        return NULL;

    struct token_t  *token = malloc(sizeof(struct token_t));

    uint32_t tok = 0;
    char     *pch = NULL;
    char     line[32];

    strcpy(line, memorycode[src]);

    pch = strtok(line, " ,");

    strcpy(token->insname, "");
    strcpy(token->dest, "");
    strcpy(token->src, "");

    while(pch != NULL) {
        if(tok == 0) {
            strcpy(token->insname, pch);
        }
        else if(tok == 1) {
            strcpy(token->src, pch);
        }
        else if(tok == 2) {
            strcpy(token->dest, pch);
        }

        pch = strtok(NULL, " ,");
        ++tok;
    }

    return token;
}
char *read_line_memcode(uint32_t src) {
    if(src >= MEM_SIZE)
        return NULL;

    return memorycode[src];
}

enum errorcode_t parse_arg_string(const char *str, uint32_t *addr, int16_t *val) {
    enum errorcode_t err = SUCCESS;
    
    if(str[0] == '#') {
        *val = (int16_t)atoi((str + 1));
    }
    else if(str[0] == '['){
        if(str[strlen(str)-2] == ']')
            return MISSING_RIGHTBRACKET;

        char tmp[strlen(str)];

        strncpy(tmp, (str+1), strlen(str)-2);

        if(addr != NULL)
            *addr = atoi(tmp) + N_REGISTERS;

        if(val != NULL)
            *val = memory[*addr + N_REGISTERS];


    }
    else if(str != NULL && isalpha(str[0])) {
        if(val != NULL) {
            uint32_t reg;

            err = get_index_reg(str, &reg);

            *val = memory[reg];
        }
        if(addr != NULL)
            err = get_index_reg(str, addr);
    }

    return err;
}

enum errorcode_t get_regdest_val(uint32_t *addr, int16_t *val, char *src, char *dest) {
    uint32_t regdest = 0;

    enum errorcode_t err;

    err = parse_arg_string(src, NULL, val);

    if(err != SUCCESS) return err;

    err = parse_arg_string(dest, &regdest, NULL);

    if(err != SUCCESS) return err;

    if(addr != NULL)
        *addr = regdest;
    
    return SUCCESS;
}

enum errorcode_t get_index_reg(const char *regname, uint32_t *reg) {
    static const char *table[N_REGISTERS] = {
        "R1",
        "R2",
        "R3",
        "R4",
        "R5",
        "R6",
        "CMP"
    };

    for(uint32_t i = 0; i < N_REGISTERS; ++i) {
        if(strcmp(table[i], regname) == 0) {
            *reg = i;
            return SUCCESS;
        }
    }

    return BADREGISTER;
} 

void set_memorycode(const char *line, uint32_t index) {
    if(index < MEM_SIZE)
        strcpy(memorycode[index], line);
}

enum errorcode_t push_pc_stack(uint32_t pc) {
    if(pcstack.index + 1 >= 256) return PCOVERFLOW;

    pcstack.mem[pcstack.index++] = pc;
    
    return SUCCESS;
}

enum errorcode_t pop_pc_stack(uint32_t *pc) {
    if(pcstack.index <= 0) return PCOVERFLOW;

    *pc = pcstack.mem[--pcstack.index];

    return SUCCESS;
}
