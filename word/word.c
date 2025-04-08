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
  word *w = malloc(sizeof *e);
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
  w->s[e->length] = (char) c;
  w->s[e->length + 1] = '\0';
  w->length++;
  return w;
}

void word_reinit(word *w, int numFiles, int file_index) {
  w->length = 0;
  w->s[0] = '\0';
  for (int i = 0; i < numFiles; i++) {
    w->in_files[i] = (i == file_index);
  }
}

bool word_is_empty(word *w) {
  return w->length == 0;
}

char *word_get(word *w) {
  return w->s;
}

size_t word_length(word *w) {
  return w->length;
}

void word_dispose(word **wptr) {
  if (*wptr) {
    free((*wptr)->in_files);
    free((*wptr)->s);
    free(*wptr);
    *wptr = nullptr;
  }
}

//void add_node(Node **head, element *e) {
  //Node *new_node = malloc(sizeof(Node));
  //new_node->e = e;
  //new_node->next = *head;
  //*head = new_node;
//}

//void free_list(Node *head) {
  //while (head) {
    //Node *tmp = head;
    //element_dispose(&head->e);
    //head = head->next;
    //free(tmp);
  //}
//}

//void print_element(element *e, int numFiles) {
  //printf("Mot: \"%s\", Présence fichiers: [", element_get(e));
  //for (int i = 0; i < numFiles; i++) {
    //printf("%s", e->in_files[i] ? "1" : "0");
    //if (i < numFiles - 1) {
      //printf(", ");
    //}
  //}
  //printf("]\n");
//}

//element *find_element(Node *head, const char *word) {
  //for (Node *n = head; n != NULL; n = n->next) {
    //if (strcmp(element_get(n->e), word) == 0) {
      //return n->e;
    //}
  //}
  //return NULL;
//}
