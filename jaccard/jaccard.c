// jaccard : partie implémentation du module jaccard

#include "jaccard.h"
#include <string.h>

#define WORD_IN_FILE "x"
#define WORD_NOT_IN_FILE "-"

struct jcrd {
  bst *tree;
  char **inputs_name;
  size_t *inter;
  bool graph;
  size_t nb_files;
};

static int put(size_t *size, const void *p) {
  printf("%s\t", element_get_string(p));
  bool *a = element_get_in_files(p);
  for (size_t k = 0; k < *size; ++k) {
    if (a[k]) {
      printf(WORD_IN_FILE);
    } else {
      printf(WORD_NOT_IN_FILE);
    }
    if (k != *size - 1) {
      printf("\t");
    }
  }
  printf("\n");
  return 0;
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
  char **f = malloc(n * sizeof(char *));
  if (p->tree == nullptr || t == nullptr || f == nullptr) {
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

element *jcrd_add(jcrd *j, element *e, size_t file_index) {
  element *t;
  if ((t = bst_add_endofpath(j->tree, e)) == e) {
    return e;
  }
  bool *in_files = element_get_in_files(t);
  if (in_files[file_index]) {
    return e;
  }
  in_files[file_index] = true;
  if (!j->graph) {
    size_t i = file_index - 1;
    for (size_t k = 0; k < file_index; ++k) {
      if (in_files[k]) {
        j->inter[i] += 1;
      }
      i += j->nb_files - 2 - k;
    }
  }
  return t;
}

void jcrd_print_graph(jcrd *j) {
  size_t k = j->nb_files;
  printf("\t");
  for (size_t i = 0; i < k; ++i) {
    printf("%s", j->inputs_name[i]);
    if (i != k - 1) {
      printf("\t");
    }
  }
  printf("\n");
  bst_dft_infix_apply_context(j->tree, -1, &k, (int (*)(void *,
      const void *))put, nullptr,
      nullptr);
}

size_t jcrd_get_nb_files(jcrd *j) {
  return j->nb_files;
}

char **jcrd_get_inputs_name(jcrd *j) {
  return j->inputs_name;
}

size_t *jcrd_get_inter(jcrd *j) {
  return j->inter;
}
