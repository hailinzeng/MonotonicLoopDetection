// RUN: %S/../../../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm %s -o %t1.bc
// RUN: %S/../../../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges %t1.bc -o %t1.bc
// RUN: %S/../../../build/bin/opt -mld %t1.bc -o - | llvm-dis | FileCheck %s

// CHECK-LABEL: entry:
#include <stdlib.h>

struct linklist {
    int value;
    linklist* next;
};

typedef struct linklist linklist_t;

bool hasloop(linklist_t* list) {
    if (list == NULL)
        return false;
    linklist_t* fast = list;
    linklist_t* slow = list;
    while (fast->next != NULL && slow->next != NULL) {
        if (fast == slow) {
            return true;
        }
        slow = slow->next;
        if (fast->next->next == NULL) {
            return false;
        }
        fast = fast->next->next;
    }
    return false;
}

int main() {
    linklist* head = NULL;
    linklist* p = NULL;

    linklist* tail = NULL;
    tail = (linklist*)malloc(sizeof(linklist_t));
    tail->value = 1;
    tail->next = NULL;

    p = (linklist*)malloc(sizeof(linklist_t));
    p->value = 2;
    p->next = tail;

    head = p;
    tail->next = head;

    return hasloop(head);
}
