#include "label.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"

enum errorcode_t search_label_and_jump(uint32_t *pc, const char *arg1) {
    for(int i = 0; labels[i].name[0] != '\0'; ++i) {
        if(strcmp(labels[i].name, arg1) == 0) {
            *pc = labels[i].pc;

            return SUCCESS;
        }
    }

    return NOLABEL;
}
