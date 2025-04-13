// jaccard : partie implémentation du module jaccard

#include "jaccard.h"
#include "holdall.h"
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define WORD_IN_FILE "x"
#define WORD_NOT_IN_FILE "-"

#define CAPACITY_MIN 70000
#define CAPACITY_MULT 2

struct jcrd {
  hashtable *table;
  holdall *hd;
  const char **inputs_name;
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

static int str_dispose(char *s) {
  free(s);
  return 0;
}

static jcrd *context(jcrd *j, [[maybe_unused]] char *s) {
  return j;
}

static int table_print(char *s, jcrd *j) {
  void *f_ptr = hashtable_search(j->table, s);
  if (f_ptr == nullptr) {
    return -1;
  }
  uint64_t f_val = (uint64_t) (uintptr_t) f_ptr;
  int size = jcrd_get_nb_files(j);
  printf("%s\t", s);
  for (int k = 0; k < size; ++k) {
    if (f_val & (1ULL << k)) {
      printf(WORD_IN_FILE);
    } else {
      printf(WORD_NOT_IN_FILE);
    }
    if (k != size - 1) {
      printf("\t");
    }
  }
  printf("\n");
  return 0;
}

jcrd *jcrd_init(const char **files, int nb_files, bool graph) {
  jcrd *p = malloc(sizeof(*p));
  if (p == nullptr) {
    return nullptr;
  }
  p->table
    = hashtable_empty((int (*)(const void *, const void *))strcmp,
      (size_t (*)(const void *))str_hashfun, 1.0);
  p->hd = holdall_empty();
  int i = ((nb_files - 1) * nb_files) / 2;
  size_t *t = calloc((size_t) i, sizeof(*t));
  size_t *c = calloc((size_t) nb_files, sizeof(size_t));
  if (p->table == nullptr || t == nullptr || c == nullptr || p->hd == nullptr) {
    hashtable_dispose(&(p->table));
    holdall_dispose(&(p->hd));
    free(t);
    free(c);
    return nullptr;
  }
  p->inputs_name = files;
  p->inter = t;
  p->graph = graph;
  p->nb_files = nb_files;
  p->cardinals = c;
  return p;
}

void jcrd_dispose(jcrd **jptr) {
  if (*jptr == nullptr) {
    return;
  }
  hashtable_dispose(&((*jptr)->table));
  holdall_apply((*jptr)->hd, (int (*)(void *))str_dispose);
  holdall_dispose(&((*jptr)->hd));
  free((*jptr)->inter);
  free((*jptr)->cardinals);
  free(*jptr);
  *jptr = nullptr;
}

int jcrd_add(jcrd *j, word *w, int file_index) {
  void *f_ptr;
  char *s = word_get(w);
  f_ptr = hashtable_search(j->table, s);
  if (f_ptr == nullptr) {
    s = malloc(word_length(w) + 1);
    if (s == nullptr) {
      return -1;
    }
    word_get_clean(w, s);
    uint64_t f_val = 1ULL << file_index;
    if (holdall_put(j->hd, s) != 0) {
      free(s);
      return -1;
    }
    if (hashtable_add(j->table, s, (void *) (uintptr_t) f_val) == nullptr) {
      free(s);
      return -1;
    }
    j->cardinals[file_index] += 1;
  } else {
    uint64_t f_val = (uint64_t) (uintptr_t) f_ptr;
    f_val |= (1ULL << file_index);
    hashtable_add(j->table, s, (void *) (uintptr_t) f_val);
  }
  return 0;
}

int jcrd_print_graph(jcrd *j) {
  int k = j->nb_files;
  printf("\t");
  for (int i = 0; i < k; ++i) {
    printf("%s", j->inputs_name[i]);
    if (i != k - 1) {
      printf("\t");
    }
  }
  printf("\n");
  holdall_sort(j->hd, (int (*)(const void *, const void *))strcmp);
  int r = holdall_apply_context(j->hd, (void *) j, (void *(*)(void *, void *))context,
      (int (*)(void *, void *))table_print);
  return r;
}

int jcrd_get_nb_files(jcrd *j) {
  return j->nb_files;
}

const char **jcrd_get_inputs_name(jcrd *j) {
  return j->inputs_name;
}

size_t *jcrd_get_inter(jcrd *j) {
  return j->inter;
}

size_t *jcrd_get_cardinals(jcrd *j) {
  return j->cardinals;
}
