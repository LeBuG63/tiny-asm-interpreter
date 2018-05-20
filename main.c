#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#define BASE_CODE       0x8000
#define N_REGISTERS     7 // 7 ne doit pas etre utilise
#define MEM_SIZE        0x8000
#define N_FUNCTIONS     7

int16_t memory[MEM_SIZE] = {0};
char  memorycode[MEM_SIZE][128];

// instruction destination source
// # -> represente une valeur numérique
// [] -> represente un emplacement dans la mémoire

struct token_t {
    char    insname[8];

    char    dest[8];
    char    src[8];
};

struct label_t {
    char        name[32];

    uint32_t    pc;
} labels[128];

enum errorcode_t {
    UNKNOWN,
    SUCCESS,
    MISSING_RIGHTBRACKET,
    OUTOFBOUNDS,
    NOLABEL
};

const char *error_msg[] = {
    "Unknown error: line %d\n",
    "",
    "Missing bracket: line %d\n",
    "Out of bounds: line %d\n",
    "Label doesn't exist: line %d"
};

enum errorcode_t builtin_add(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_sub(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_prt(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_store(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_cmp(char *arg1, char *arg2, uint32_t *);
enum errorcode_t builtin_jge(char *arg1, char *arg2, uint32_t *);
enum errorcode_t builtin_jra(char *arg1, char *arg2, uint32_t *);

struct ptrfunc_t {
    char *name;
    enum errorcode_t (*func)(char *, char *, uint32_t *);
};

const struct ptrfunc_t builtin_functions[] = {
    {"ADD", builtin_add},
    {"SUB", builtin_sub},
    {"PRINT", builtin_prt},
    {"STORE", builtin_store},
    {"CMP", builtin_cmp},
    {"JGE", builtin_jge},
    {"JRA", builtin_jra}
};

struct token_t *read_from_memcode(uint32_t src) {
    if(src > MEM_SIZE) 
        return NULL;

    struct token_t  *token = malloc(sizeof(struct token_t));

    uint32_t tok = 0;
    char     *pch = NULL;
    char     line[32];

    strcpy(line, memorycode[src]);

    pch = strtok(line, " ,");

    while(pch != NULL) {
        if(tok == 0) {
            strcpy(token->insname, pch);
        }
        else if(tok == 1) {
            strcpy(token->dest, pch);
        }
        else if(tok == 2) {
            strcpy(token->src, pch);
        }

        pch = strtok(NULL, " ,");
        ++tok;
    }

    return token;
}

bool set_mem(uint32_t addr, uint32_t value) {
    if(addr > MEM_SIZE)
        return false;

    memory[addr] = value;

    return true;
}

int get_index_reg(const char *regname) {
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
        if(strcmp(table[i], regname) == 0)
            return i;
    }

    return -1;
} 

enum errorcode_t get_regdest_val(int16_t **mem, int16_t *val, char *src, char *dest) {
    uint32_t regdest = 0;
    uint32_t regsrc = 0;

    if(src[0] == '#') {
        *val = (int16_t)atoi((src + 1));
    }
    else if(src[0] == '['){
        if(src[strlen(src)-2] == ']')
            return MISSING_RIGHTBRACKET;

        char tmp[strlen(src)];

        strncpy(tmp, (src+1), strlen(src)-2);

        regsrc = atoi(tmp);

        *val = memory[regsrc + N_REGISTERS];
    }
    else if(src != NULL) {
        *val = memory[get_index_reg(src)];
    }

    if(dest[0] == '['){
        if(dest[strlen(dest)-2] == ']')
            return MISSING_RIGHTBRACKET;

        char tmp[strlen(dest)];

        strncpy(tmp, (dest+1), strlen(dest)-2);

        regdest = atoi(tmp) + N_REGISTERS;
    }
    else {
        regdest = get_index_reg(dest);
    }

    *mem = &memory[regdest];

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    uint32_t    pc = 0;
    uint32_t    nlabel = 0;
    FILE        *fp = NULL;

    fp = fopen(argv[1], "r");

    if(!fp)
        return 1;

    char line[128] = {0};
    
    while(!feof(fp)) {
        char formated_line[128];

        fgets(line, 128, fp);

        line[strlen(line) - 1] = '\0';

        for(int i = 0; line[i] != '\0'; ++i) {
            while((line[i] == ' ' && line[i + 1] == ' ') || (line[i] == '\t'))
                ++i;
            line[i] = toupper(line[i]);
        }
        strcpy(memorycode[pc], line);

        if(line[strlen(line) - 1] == ':') {
            strncpy(labels[nlabel].name, line, strlen(line) - 1);
            labels[nlabel].pc = pc;
            ++nlabel;
        }

        pc++;
    }

    enum errorcode_t    err = SUCCESS;
    uint32_t            end = pc - 1;
    pc = 0;

    while(pc != end && err == SUCCESS) {
        struct token_t  *tok = read_from_memcode(pc);

        if(strcmp(tok->insname, "RET") == 0)
            break;

        for(int i = 0; i < N_FUNCTIONS; ++i) {
            if(strcmp(builtin_functions[i].name, tok->insname) == 0) {
                err = builtin_functions[i].func(tok->src, tok->dest, &pc);
            }
        }
        free(tok);

        if(err != SUCCESS) {
            printf(error_msg[err], pc + 1);
        }
        pc++;
    }

    return 0;
}

enum errorcode_t builtin_sub(char *src, char *dest, uint32_t *pc) {
    int16_t *mem = NULL;
    int16_t val;

    enum errorcode_t err;

    err = get_regdest_val(&mem, &val, dest, src);

    if(err == SUCCESS)
        *mem -= val;

    return err;
}

enum errorcode_t builtin_add(char *src, char *dest, uint32_t *pc) {
    int16_t *mem = NULL;
    int16_t val;

    enum errorcode_t err;

    err = get_regdest_val(&mem, &val, dest, src);

    if(err == SUCCESS)
        *mem += val;

    return err;
}

enum errorcode_t builtin_store(char *src, char *dest, uint32_t *pc) {
    int16_t *mem = NULL;
    int16_t val;

    enum errorcode_t err;

    err = get_regdest_val(&mem, &val, dest, src);

    if(err == SUCCESS)
        *mem = val;

    return err;
}

enum errorcode_t builtin_prt(char *src, char *dest, uint32_t *pc) {
    (void *)src;

    int vdest = 0;

    if(isalpha(dest[0])) {
        vdest = get_index_reg(dest);

        if(vdest < 0)
            return OUTOFBOUNDS;

        printf("ADDR[%d]: %d\n", vdest, memory[vdest]);
    }
    else {
        vdest = (uint16_t)atoi(dest) + N_REGISTERS;

        printf("ADDR[%d]: %d\n", vdest - N_REGISTERS, memory[vdest]);
    }


    return SUCCESS;
}

enum errorcode_t builtin_cmp(char *arg1, char *arg2, uint32_t *pc) { 
    int16_t *mem = NULL;
    int16_t val;

    enum errorcode_t err;

    err = get_regdest_val(&mem, &val, arg1, arg2);

    if(err == SUCCESS) {
        if(*mem == val)     memory[get_index_reg("CMP")] = 0;
        else if(*mem < val) memory[get_index_reg("CMP")] = 1;
        else                memory[get_index_reg("CMP")] = 2;
    }

    return err;
}

enum errorcode_t builtin_jge(char *arg1, char *arg2, uint32_t *pc) {
    if(memory[get_index_reg("CMP")] <= 1) {
        for(int i = 0; labels[i].name[0] != '\0'; ++i) {
            if(strcmp(labels[i].name, arg2) == 0) {
                *pc = labels[i].pc;
                return SUCCESS;
            }
        }
        return NOLABEL;
    }
    return SUCCESS;
}

enum errorcode_t builtin_jra(char *arg1, char *arg2, uint32_t *pc) {
    for(int i = 0; labels[i].name[0] != '\0'; ++i) {
        if(strcmp(labels[i].name, arg2) == 0) {
            *pc = labels[i].pc;
            return SUCCESS;
        }
    }
    return NOLABEL;
}
