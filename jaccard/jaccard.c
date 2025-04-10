// jaccard : partie implémentation du module jaccard

#include "jaccard.h"
#include <string.h>

struct jcrd {
  bst *tree;
  char **inputs_name;
  size_t *inter;
  bool graph;
  size_t nb_files;
  size_t *cardinals;
};

static void put(const void *p) {
  printf("%s", element_get_string(p));
}


jcrd *jcrd_init(stack *inputs, bool graph) {
  jcrd *p = malloc(sizeof(*p));
  if (p == nullptr) {
    return nullptr;
  }
  p->tree = bst_empty((int (*)(const void *, const void *))element_compar);
  size_t n = stack_height(inputs);
  size_t i = ((n - 1) * n) / 2;
  size_t *t = malloc(i * sizeof(*t));
  size_t *c = calloc(p->nb_files, sizeof(size_t));
  char **f = malloc(n * sizeof(char *));
  if (p->tree == nullptr || t == nullptr || f == nullptr || c == nullptr) {
    bst_dispose(&(p->tree));
    free(t);
    free(f);
    return nullptr;
  }
  for (size_t k = 0; k < i; ++k) {
    t[k] = 0;
  }
  for (size_t k = n; k > 0; --k) {
    f[k - 1] = stack_pop(inputs);
  }
  p->inputs_name = f;
  p->inter = t;
  p->graph = graph;
  p->nb_files = n;
  p->cardinals = c;
  return p;
}

void jcrd_dispose(jcrd **jptr) {
  if (*jptr == nullptr) {
    return;
  }
  bst_dispose(&((*jptr)->tree));
  free((*jptr)->inputs_name);
  free((*jptr)->inter);
  free((*jptr)->cardinals);
  free(*jptr);
  *jptr = nullptr;
}

element *jcrd_add(jcrd *j, element *e, size_t file_index) {
  printf("\t ---%s\n", element_get_string(e));
  bst_repr_graphic(j->tree, put);
  printf("\n---\n");
  element *t = bst_add_endofpath(j->tree, e);
  bst_repr_graphic(j->tree, put);
  printf("\n\n\n\n\n-------------------------------\n");
  if (t == e) {
     j->cardinals[file_index]+=1;
    return e;
  }
  bool *in_files = element_get_in_files(t);
  if (in_files[file_index]) {
    return e;
  }
  in_files[file_index] = true;
       j->cardinals[file_index]+=1;

  if(!j->graph) {
    size_t i = file_index - 1;
    for(size_t k = 0; k < file_index; ++k) {
      if (in_files[k]) {
        j->inter[i] += 1;
      }
      i += j->nb_files - 2 - k;
    }
  }

  return t;
}



size_t jcrd_get_nb_files(jcrd *j){
  return j->nb_files;
}

char **jcrd_get_inputs_name(jcrd *j){
  return j->inputs_name;
}

size_t *jcrd_get_inter(jcrd *j){
  return j->inter;
}

size_t *jcrd_get_cardinals(jcrd *j) {
  return j->cardinals;
}












