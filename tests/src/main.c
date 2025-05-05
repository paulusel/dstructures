#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "dstruct/dlinked_list.h"

int fd = -1;
void test_dlist();
int random_number();

int main(int argc, char *argv[]) {
    fd = open("/dev/urandom", O_RDONLY);
    if(fd < 0) {
        printf("failed to open random_number source\n");
        return 1;
    }

    if(argc < 2) {
        printf("no test specified: --dlist, --vector, --pqueue, --deque, --htable, --rbtree\n");
        return 1;
    }

    if(strcmp(argv[1], "--dlist") == 0) {
        test_dlist();
    }
    else {
        printf("unrecognized test\n");
        return 1;
    }

    close(fd);
    return 0;
}

void test_dlist() {
    dlist  *list = dlist_create();
    for(int i = 0; i<100; i++) {
        int random = random_number();

        int indx = list->size ? random % list->size : 0;
        node *nd = list->end->next;
        for(int i = 0; i<indx; ++i) nd = nd->next;

        if(random % 2) {
            int *val = (int*)malloc(sizeof(int));
            *val = random;
            dlist_insert(list, nd, val);
        }
        else {
            if(nd != list->end) free(nd->data);
            dlist_remove(list, nd);
        }
    }

    node *nd = list->end->next;
    while(nd != list->end) {
        free(nd->data);
        nd = nd->next;
    }
    dlist_destrory(list);
}

int random_number() {
    int random_num;
    read(fd, &random_num, sizeof(random_num));
    return abs(random_num);
}
