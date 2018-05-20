#ifndef __LABEL_H__
#define __LABEL_H__

#include <stdint.h>

struct label_t {
    char        name[32];

    uint32_t    pc;
} labels[128];

enum errorcode_t search_label_and_jump(uint32_t *pc, const char *arg1);

#endif // __LABEL_H__