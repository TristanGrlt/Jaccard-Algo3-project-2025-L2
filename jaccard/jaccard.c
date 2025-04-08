// jaccard : partie implémentation du module jaccard

#include "jaccard.h"

jcrd *jcrd_init(stack *inputs, bool graph) {
  jcrd *p = malloc(sizeof(*p));
  if (p == nullptr) {
    return nullptr;
  }
  p->tree = bst_empty((int (*)(const void *, const void *))element_compar);
  size_t n = stack_height(inputs);
  size_t i = ((n - 1) * n) / 2;
  size_t *t = malloc(i * sizeof(*t));
  char **f = malloc(n * sizeof(char *));
  if (p->tree == nullptr || t == nullptr || f == nullptr) {
    bst_dispose(&(p->tree));
    free(t);
    free(f);
  }
  for (size_t k = 0; k < i; ++k) {
    t[k] = 0;
  }
  for (size_t k = 0; k < n; ++k) {
    f[k] = stack_pop(inputs);
  }
  p->inputs_name = f;
  p->inter = t;
  p->graph = graph;
  return p;
}

void jcrd_dispose(jcrd **jptr) {
  if (*jptr == nullptr) {
    return;
  }
  bst_dispose(&((*jptr)->tree));
  free((*jptr)->inputs_name);
  free((*jptr)->inter);
  free(*jptr);
  *jptr = nullptr;
}
