#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include <stdint.h>

#include "memory.h"

#define N_FUNCTIONS     12

struct ptrfunc_t {
    char *name;
    enum errorcode_t (*func)(char *, char *, uint32_t *);
};

enum errorcode_t builtin_add(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_sub(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_prt(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_move(char *src, char *dest, uint32_t *);
enum errorcode_t builtin_cmp(char *arg1, char *arg2, uint32_t *);
enum errorcode_t builtin_jge(char *arg1, char *arg2, uint32_t *);
enum errorcode_t builtin_jmp(char *arg1, char *arg2, uint32_t *);
enum errorcode_t builtin_call(char *arg1, char *arg2, uint32_t *pc);
enum errorcode_t builtin_jle(char *arg1, char *arg2, uint32_t *pc);
enum errorcode_t builtin_ret(char *arg1, char *arg2, uint32_t *pc);
enum errorcode_t builtin_jl(char *arg1, char *arg2, uint32_t *pc);
enum errorcode_t builtin_jg(char *arg1, char *arg2, uint32_t *pc);

int16_t val_cmp(void);
enum errorcode_t condition(bool cond, const char *arg1, uint32_t *pc);

#endif // __BUILTIN_H__