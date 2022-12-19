////////////////////////////////////////////////////
// vector - A C library adding c++ sting vectors  //
// Copyright:        by NewDawn0 (Tom) 11.12.2022 //
// Version: Header only                           //
// License: MIT                                   //
////////////////////////////////////////////////////

// Ispired by "https://markkevinbaltazar.medium.com/lets-make-a-dynamic-array-vector-in-c-16dbe6d72a79

// include guard
#ifndef VECTOR_H
#define VECTOR_H

// libs
#include <stdlib.h>

// definitions
#define VECTOR_INIT_LIM 5

// Vector typedef
typedef struct Vector {
    void **data;
    int lim;
    int count;
    void (*push_back)(struct Vector*, void*);
    void (*rm)(struct Vector*, int);
    void (*free)(struct Vector*);
} Vector;

// Vector Init method
void vectorInit(Vector *vector);

// include guard
#endif

// add data
void vectorPushBack(Vector *self, void *data) {
    // resize if neccesary
    if (self->count == self->lim) {
        self->lim = self->lim * 2;
        self->data = realloc(self->data, sizeof(void*) *self->lim);
    }
    // add data
    self->data[self->count] = data;
    self->count++;
}
// rm data
void vectorRm(Vector *self, int index) {
    if (index > -1 && index < self->count) {
        // swap the last item to the index to be removed
        // then remove the last item
        self->data[index] = self->data[self->count-1];
        self->data[self->count-1] = NULL;
        self->count --;
    }
}
// free
void vectorFree(Vector *self) {
    if (self->data) {
        free(self->data);
        self->data = NULL;
    }
}
// init vector
inline void vectorInit(Vector *vec) {
    vec->lim = VECTOR_INIT_LIM;
    vec->count = 0;
    vec->push_back = vectorPushBack;
    vec->rm = vectorRm;
    vec->free = vectorFree;
    vec->data = malloc(sizeof(void*) * vec->lim);
}
