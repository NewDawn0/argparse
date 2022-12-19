#include "vec.h"
#include <stdio.h>

void vecPrint(Vector *vec);
int main(int argc, char *argv[]) {
    Vector vec;
    vectorInit(&vec);
    vec.push_back(&vec, "hi");
    vec.push_back(&vec, "1");
    vec.push_back(&vec, "lol");
    vecPrint(&vec);
    vec.rm(&vec, 1);
    vecPrint(&vec);
    
    vec.free(&vec);
}
void vecPrint(Vector *vec) {
    printf("Count %d\n", vec->count);
    printf("vec : [ ");
    for (int i=0;i<vec->count;i++) {
        printf("%s ", (char*)vec->data[i]);
    } printf("];\n");
}
