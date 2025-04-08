// word.c
#include "word.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CAPACITY_MIN 26
#define CAPACITY_MULT 2

struct word {
  char *s;
  size_t length;
  size_t capacity;
};

word *word_init() {
  word *w = malloc(sizeof *w);
  if (w == nullptr) {
    return nullptr;
  }
  w->s = malloc(CAPACITY_MIN);
  if (w->s == nullptr) {
    free(w);
    return nullptr;
  }
  w->s[0] = '\0';
  w->length = 0;
  w->capacity = CAPACITY_MIN;
  return w;
}

void *word_add(word *w, int c) {
  if (w->length == w->capacity - 1) {
    void *a = NULL;
    if (w->capacity * sizeof *w->s <= SIZE_MAX / CAPACITY_MULT) {
      a = realloc(w->s, w->capacity * CAPACITY_MULT);
    }
    if (a == nullptr) {
      return nullptr;
    }
    w->s = a;
    w->capacity *= CAPACITY_MULT;
  }
  w->s[w->length] = (char) c;
  w->s[w->length + 1] = '\0';
  w->length++;
  return w;
}

void word_reinit(word *w) {
  w->length = 0;
  w->s[0] = '\0';
}

bool word_is_empty(word *w) {
  return w->length == 0;
}

void word_get(const word *w, char *dest) {
  for(size_t i = 0 ; i <= word_length(w); ++i) {
    dest[i] = w->s[i];
  }
}

size_t word_length(const word *w) {
  return w->length;
}

void word_dispose(word **wptr) {
  if (*wptr) {
    free((*wptr)->s);
    free(*wptr);
    *wptr = nullptr;
  }
}
