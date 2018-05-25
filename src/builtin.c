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

    uint32_t vdest = 0;

    if(isalpha(src[0])) {
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

enum errorcode_t builtin_prtc(char *src, char *dest, uint32_t *pc) {
    (void)pc;
    (void)src;

    uint32_t vdest = 0;

    if(isalpha(src[0])) {
        int16_t val;

        enum errorcode_t ret = get_value_memory(&val, vdest);
        
        if(ret != SUCCESS)
            return ret;

        printf("%c", val);
    }
    else {
        vdest = (uint16_t)atoi(dest) + N_REGISTERS;

        int16_t val;

        enum errorcode_t ret = get_value_memory(&val, vdest);

        if(ret != SUCCESS)
            return ret;

        printf("%c", val);
    }

    return SUCCESS;
}

enum errorcode_t builtin_prts(char *src, char *dest, uint32_t *pc) {
    (void)pc;
    (void)src;

    uint32_t begaddr = 0;
    uint32_t endaddr = 0;

    enum errorcode_t err;

    err = parse_arg_string(src, &begaddr, NULL);

    if(err != SUCCESS)
        return err;

    err = parse_arg_string(dest, &endaddr, NULL);
    
    if(err != SUCCESS)
        return err;

    for(uint32_t i = begaddr; i <= endaddr && err == SUCCESS; ++i)  {
        int16_t val = 0;
        err = get_value_memory(&val, i);
        
        if(err == SUCCESS)
            putchar((char)val);
    }

    return err;
}



enum errorcode_t builtin_cmp(char *arg1, char *arg2, uint32_t *pc) { 
    (void)pc;

    int16_t     val;

    enum errorcode_t err;

    err = parse_arg_string(arg2, NULL, &val);

    if(err == SUCCESS) {
        int16_t     val1;

        uint32_t addrcmp;
        
        err = get_index_reg("CMP", &addrcmp);
        
        if(err != SUCCESS)
            return err;

        err = parse_arg_string(arg1, NULL, &val1);

        if(val1 == val)     err = set_mem(addrcmp, 0);
        else if(val1 < val) err = set_mem(addrcmp, 1);
        else                err = set_mem(addrcmp, 2);
    }

    return err;
}


enum errorcode_t builtin_aski(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;
    (void)pc;
    
    enum errorcode_t err;

    uint32_t    addr = 0;
    int16_t     val = 0;

    err = parse_arg_string(arg1, &addr, NULL);

    if(err != SUCCESS)
        return err;

    putchar('?');
    scanf("%hi", &val);

    err = set_mem(addr, val);

    return err;
}

enum errorcode_t builtin_askc(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;
    (void)pc;

    enum errorcode_t err;

    uint32_t    addr = 0;
    char        val = 0;

    err = parse_arg_string(arg1, &addr, NULL);

    if(err != SUCCESS)
        return err;

    putchar('?');
    scanf("%c", &val);

    err = set_mem(addr, (int16_t)val);

    return err;
}

enum errorcode_t builtin_jge(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    int16_t val = val_cmp();

    return condition(val == 0 || val == 2, arg1, pc);
}

enum errorcode_t builtin_jle(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    int16_t val = val_cmp();

    return condition(val == 0 || val == 1, arg1, pc);
}

enum errorcode_t builtin_jl(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    int16_t val = val_cmp();

    return condition(val == 1, arg1, pc);
}

enum errorcode_t builtin_jg(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    int16_t val = val_cmp();

    return condition(val == 2, arg1, pc);
}

enum errorcode_t builtin_jmp(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    enum errorcode_t err = search_label_and_jump(pc, arg1);
    
    return err;
}

enum errorcode_t builtin_call(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg2;

    enum errorcode_t err = push_pc_stack(*pc);
    
    if(err == SUCCESS) {
        err = builtin_jmp(arg1, arg2, pc);
    }

    return err;
}

enum errorcode_t builtin_ret(char *arg1, char *arg2, uint32_t *pc) {
    (void)arg1;
    (void)arg2;
    
    pop_pc_stack(pc);

    return SUCCESS;
}

int16_t val_cmp(void) {
    int16_t val;
    
    parse_arg_string("CMP", NULL, &val);
    
    return val;
} 

enum errorcode_t condition(bool cond, const char *arg1, uint32_t *pc) {
    int16_t     val;
    uint32_t    addr;

    enum errorcode_t err;

    err = get_index_reg("CMP", &addr);

    if(err != SUCCESS)
        return err;

    err = get_value_memory(&val, addr);
    
    if((cond) && err == SUCCESS) {
        err = push_pc_stack(*pc);
        if(err != SUCCESS)
            return err;

        err = search_label_and_jump(pc, arg1);

        if(err == SUCCESS)
            return SUCCESS;

        return NOLABEL;
    }
    
    return err;
}
