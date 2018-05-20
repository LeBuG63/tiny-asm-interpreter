#ifndef __LABEL_H__
#define __LABEL_H__

#include <stdint.h>

struct label_t {
    char        name[32];

    uint32_t    pc;
} labels[128];

#endif // __LABEL_H__