#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct _llist{
    int32_t val;
    struct _llist *next;
}llist;

llist* ll_init(void) {
    llist *p = malloc(sizeof(llist));
    if(p == NULL) {
        return NULL;
    }
    p->next = NULL;
    return p;
}



int ll_add(llist* p, int32_t val, int order) {
    while((order--) > 1 && p) {
        p = p->next;
    }
    if(p != NULL) {
        llist *add = malloc(sizeof(llist));
        if(add) {
            add->val = val;
            add->next = p->next;
            p->next = add;
            return 0;
        } 
    }
    return -1;
}

int ll_add_tail(llist* p, int32_t val) {
    while(p != NULL) {
        p = p->next;
    }
}


int ll_del(llist*p, int order) {
    while((order--) > 1 && p) {
        p = p->next;
    }   
    if(p->next != NULL) {
        llist *del = p->next;
        p->next = p->next->next;
        free(del);
        return 0;
    }
    return -1;


}

void ll_print(llist* p) {
    int i = 1;
    p = p->next;
    while(p != NULL) {
        printf("#%d val is %d\n\n", i++, p->val);
        p = p->next;
    }
}


int main(int argc, char** argv) {
    llist *p = ll_init();
    int i;
    for(i = 1; i < 6; i++) {
        ll_add(p, i*2, i);
    }
    ll_print(p);
    
    ll_del(p, 3);
    ll_print(p);
//    
//    ll_del(p, 3);
//    
//    ll_print(p);  
    return 0;
}







