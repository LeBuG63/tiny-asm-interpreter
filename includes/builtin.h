#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include <stdint.h>

#include "memory.h"

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
enum errorcode_t builtin_jra(char *arg1, char *arg2, uint32_t *);

#endif // __BUILTIN_H__