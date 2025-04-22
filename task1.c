#include <stdio.h>
#include <stdlib.h>

typedef struct RCObject {
    int refcount;
    struct RCObject* partner;
} RCObject;

// Виділяє новий RCObject з початковим значенням лічильника посилань 1
RCObject* rc_new() {
    RCObject* obj = (RCObject*)malloc(sizeof(RCObject));
    if (!obj) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    obj->refcount = 1;
    obj->partner = NULL;
    return obj;
}

// Збільшує лічильник посилань
void rc_retain(RCObject* obj) {
    if (obj) {
        obj->refcount++;
    }
}

// Зменшує лічильник посилань і звільняє пам’ять, якщо він досягає нуля
// У циклічних структурах лічильник ніколи не досягає нуля, тому відбувається витік пам’яті
void rc_release(RCObject* obj) {
    if (!obj) return;
    obj->refcount--;
    if (obj->refcount == 0) {
        RCObject* p = obj->partner;
        free(obj);

        rc_release(p);
    }
}

int main() {
    RCObject *a = rc_new();
    RCObject *b = rc_new();

    a->partner = b;
    rc_retain(b);
    b->partner = a;
    rc_retain(a);

    printf("Created two objects with cyclic references\n");
    
    // Звільняємо зовнішні посилання
    rc_release(a);
    rc_release(b);

    return 0;
}
