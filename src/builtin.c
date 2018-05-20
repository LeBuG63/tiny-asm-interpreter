#include "builtin.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "label.h"

enum errorcode_t builtin_sub(char *src, char *dest, uint32_t *pc) {
    (void)pc;

    uint32_t    addr;
    int16_t     val;
    int16_t     tmp;

    enum errorcode_t err;

    err = get_regdest_val(&addr, &val, src, dest);

    if(err == SUCCESS) {
        err = get_value_memory(&tmp, addr);
        set_mem(addr, tmp - val);
    }

    return err;
}

enum errorcode_t builtin_add(char *src, char *dest, uint32_t *pc) {
    (void)pc;

    uint32_t    addr;
    int16_t     val;
    int16_t     tmp;

    enum errorcode_t err;

    err = get_regdest_val(&addr, &val, src, dest);

    if(err == SUCCESS) {
        err = get_value_memory(&tmp, addr);
        set_mem(addr, tmp + val);
    }
    return err;
}

enum errorcode_t builtin_move(char *src, char *dest, uint32_t *pc) {
    (void)pc;

    uint32_t    addr;
    int16_t     val;
    int16_t     tmp;

    enum errorcode_t err;

    err = get_regdest_val(&addr, &val, src, dest);

    if(err == SUCCESS) {
        set_mem(addr, val);
    }

    return err;
}

enum errorcode_t builtin_prt(char *src, char *dest, uint32_t *pc) {
    (void)pc;
    (void)src;

    int vdest = 0;

    if(isalpha(src[0])) {
        vdest = get_index_reg(src);
        
        int16_t val;

        enum errorcode_t ret = get_value_memory(&val, vdest);

        if(ret != SUCCESS)
            return ret;

        printf("ADDR[%d]: %d\n", vdest, val);
    }
    else {
        vdest = (uint16_t)atoi(dest) + N_REGISTERS;

        int16_t val;

        enum errorcode_t ret = get_value_memory(&val, vdest);

        if(ret != SUCCESS)
            return ret;

        printf("ADDR[%d]: %d\n", vdest - N_REGISTERS, val);
    }


    return SUCCESS;
}

enum errorcode_t builtin_cmp(char *arg1, char *arg2, uint32_t *pc) { 
    (void)pc;

    int16_t     val;
    int16_t     tmp;

    enum errorcode_t err;

    err = parse_arg_string(arg2, NULL, &val);

    if(err == SUCCESS) {
        int16_t     val1;

        uint32_t addrcmp = get_index_reg("CMP");

        err = parse_arg_string(arg1, NULL, &val1);

        if(val1 == val)     err = set_mem(addrcmp, 0);
        else if(val1 < val) err = set_mem(addrcmp, 1);
        else                err = set_mem(addrcmp, 2);
    }

    return err;
}

enum errorcode_t builtin_jge(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    int16_t val;

    enum errorcode_t err;

    err = get_value_memory(&val, get_index_reg("CMP"));
    
    if(val <= 1 && err == SUCCESS) {
        for(int i = 0; labels[i].name[0] != '\0'; ++i) {
            if(strcmp(labels[i].name, arg1) == 0) {
                *pc = labels[i].pc;
                return SUCCESS;
            }
        }
        return NOLABEL;
    }
    
    return err;
}

enum errorcode_t builtin_jra(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    for(int i = 0; labels[i].name[0] != '\0'; ++i) {
        if(strcmp(labels[i].name, arg1) == 0) {
            *pc = labels[i].pc;
            return SUCCESS;
        }
    }

    return NOLABEL;
}
