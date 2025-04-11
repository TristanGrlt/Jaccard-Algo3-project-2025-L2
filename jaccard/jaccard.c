// jaccard : partie implémentation du module jaccard

#include "jaccard.h"
#include <string.h>

#define WORD_IN_FILE "x"
#define WORD_NOT_IN_FILE "-"

struct jcrd {
  hashtable *table;
  char **inputs_name;
  size_t *inter;
  bool graph;
  int nb_files;
  size_t *cardinals;
};

static size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

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
  p->table = hashtable_empty((int (*)(const void *, const void *))strcmp, (size_t (*)(const void *))str_hashfun, 1.0);
  size_t n = stack_height(inputs);
  size_t i = ((n - 1) * n) / 2;
  size_t *t = malloc(i * sizeof(*t));
  size_t *c = calloc(p->nb_files, sizeof(size_t));
  char **f = malloc(n * sizeof(char *));
  if (p->tree == nullptr || t == nullptr || f == nullptr || c == nullptr) {
    hashtable_dispose(&(p->table));
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
  hashtable_dispose(&((*jptr)->table));
  free((*jptr)->inputs_name);
  free((*jptr)->inter);
  free((*jptr)->cardinals);
  free(*jptr);
  *jptr = nullptr;
}

int jcrd_add(jcrd *j, word *w, size_t file_index) {
  uint64_t f;
  char *s = word_get(w);
  if ((&f = hashtable_search(j->table, s)) == nullptr) {
    s = malloc(word_length(w) + 1);
    if(s == nullptr) {
      return -1;
    }
    word_get_clean(w, s);
    f = 0;
    j->cardinals[file_index]+=1;
  } else {
    f |= (1ULL << file_index);
  }
  if (hashtable_add(j->table, s, &f) == nullptr) {
      free(s);
      return -1;
  }
  return 0;
}


  bool *in_files = element_get_in_files(t);
  if (in_files[file_index]) {
    return e;
  }
  in_files[file_index] = true;
  //if(!j->graph) {
    //j->cardinals[file_index]+=1;
    //size_t i = file_index - 1;
    //for (size_t k = 0; k < file_index; ++k) {
      //if (in_files[k]) {
        //j->inter[i] += 1;
      //}
      //i += j->nb_files - 2 - k;
    //}
  //}
  return t;
}

//void jcrd_print_graph(jcrd *j) {
  //size_t k = j->nb_files;
  //printf("\t");
  //for (size_t i = 0; i < k; ++i) {
    //printf("%s", j->inputs_name[i]);
    //if (i != k - 1) {
      //printf("\t");
    //}
  //}
  //printf("\n");
  //bst_dft_infix_apply_context(j->tree, -1, &k, (int (*)(void *,
      //const void *))put, nullptr,
      //nullptr);
//}

int jcrd_get_nb_files(jcrd *j) {
  return j->nb_files;
}

char **jcrd_get_inputs_name(jcrd *j) {
  return j->inputs_name;
}

size_t *jcrd_get_inter(jcrd *j) {
  return j->inter;
}

size_t *jcrd_get_cardinals(jcrd *j) {
  return j->cardinals;
}

