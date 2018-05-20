
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "builtin.h"
#include "label.h"
#include "memory.h"

const char *error_msg[] = {
    "Unknown error: line %d\n",
    "",
    "Missing bracket: line %d\n",
    "Out of bounds: line %d\n",
    "Label doesn't exist: line %d",
    "PC Overflow"
};

struct ptrfunc_t builtin_function[] = {
    {"ADD", builtin_add},
    {"SUB", builtin_sub},
    {"PRINT", builtin_prt},
    {"MOVE", builtin_move},
    {"CMP", builtin_cmp},
    {"JGE", builtin_jge},
    {"JRA", builtin_jmp},
    {"RET", builtin_ret},
    {"JLE", builtin_jle},
    {"JL",  builtin_jl},
    {"JG",  builtin_jg},
    {"CALL", builtin_call}
};

int main(int argc, char *argv[]) {
    (void)argc;

    uint32_t    pc = 0;
    uint32_t    nlabel = 0;
    FILE        *fp = NULL;

    fp = fopen(argv[1], "r");

    if(!fp)
        return 1;

    char line[128] = {0};
    
    while(!feof(fp)) {
        int  i = 0;
        int  k = 0;
        int  j = 0;
        char formatedline[128] = {0};

        fgets(line, 128, fp);

        line[strlen(line) - 1] = '\0';

        for(i = 0; isspace(line[i]); ++i)
            ;

        for(j = strlen(line)-1; isspace(line[j]); --j)
            ;

        line[j+1] = '\0';

        if(line[0] == '\0') continue;

        for(k = 0; line[i] != '\0'; ++i, ++k) {
            while(isspace(line[i]) && isspace(line[i + 1]))
                ++i;
            formatedline[k] = toupper(line[i]);
        }

        formatedline[strlen(formatedline)] = '\0';
        if(formatedline[0] == ';') continue;
        
        set_memorycode(formatedline, pc);

        if(formatedline[strlen(formatedline) - 1] == ':') {
            strncpy(labels[nlabel].name, formatedline, strlen(formatedline) - 1);
            
            labels[nlabel].pc = pc;
            ++nlabel;
        }

        pc++;
    }

    enum errorcode_t    err = SUCCESS;
    uint32_t            end = pc - 1;
    pc = 0;

    while(pc <= end && err == SUCCESS) {
        struct token_t  *tok = read_from_memcode(pc);

        if(strcmp(tok->insname, "RETS") == 0)
            break;

        for(int i = 0; i < N_FUNCTIONS; ++i) {
            if(strcmp(builtin_function[i].name, tok->insname) == 0) {
                err = builtin_function[i].func(tok->src, tok->dest, &pc);
            }
        }

        free(tok);

        
        pc++;

        if(err != SUCCESS) {
            printf(error_msg[err], pc + 1);
        }
    }

    return 0;
}
