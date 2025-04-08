// element.c : partie implémentation du module element.h
#include "element.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct element {
  const char *s;
  bool *in_files;
};

element *element_init(const word *w, size_t numFiles, size_t file_index) {
  if (file_index >= numFiles) {
    return nullptr;
  }
  element *e = malloc(sizeof *e);
  if (e == nullptr) {
    return nullptr;
  }
  char *s = malloc(word_length(w) + 1);
  if(s == nullptr) {
    return nullptr;
  }
  word_get(w, s);
  e->s = s;
  e->in_files = malloc((size_t) numFiles * sizeof(bool));
  if (e->in_files == nullptr) {
    free((char *)e->s);
    free(e);
    return nullptr;
  }
  for (size_t i = 0; i < numFiles; i++) {
    e->in_files[i] = (i == file_index);
  }
  return e;
}

void element_dispose(element **eptr) {
  if (*eptr != nullptr) {
    free((*eptr)->in_files);
    free((char *)(*eptr)->s);
    free(*eptr);
    *eptr = nullptr;
  }
}

int element_compar(const element *e1, const element *e2) {
  return strcmp(e1->s, e2->s);
}

char *element_get_string(const element *e){
  return (char *)e->s;
}

bool *element_get_in_files(const element *e) {
  return e->in_files;
}














