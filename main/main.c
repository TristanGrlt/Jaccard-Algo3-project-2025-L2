#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "opt.h"
#include "jaccard.h"

#define ALLOC_ERROR "An allocation error occured"
#define FERROR "An error occured while treating file: "
#define ERROR_ -1

int main(int argc, char *argv[]) {
  int r = EXIT_SUCCESS;
  opt *option = opt_empty();
  if (option == nullptr) {
    r = EXIT_FAILURE;
    goto opt_dispose;
  }
  opt_create(option, argv, argc);
  jcrd *j = jcrd_init(opt_get_files(option), false);
  if (j == nullptr) {
    goto error_capacity;
  }
  word *w = word_init();
  if (w == nullptr) {
    goto error_capacity;
  }
  printf("%zu\n", jcrd_get_nb_files(j));
  for (int k = 0; k < jcrd_get_nb_files(j); ++k) {
    FILE *f = fopen(jcrd_get_inputs_name(j)[k], "r");
    if (f == nullptr) {
      fprintf(stderr, FERROR "%s\n", jcrd_get_inputs_name(j)[k]);
      r = ERROR_;
      continue;
    }
    int c;
    while ((c = fgetc(f)) != EOF) {
      if (isspace(c)) {
        if (jcrd_add(j, w, k) != 0) {
          goto error_capacity;
        }
        word_reinit(w);
      } else {
        if (word_add(w, c) == nullptr) {
          goto error_capacity;
        }
      }
    }
    if (word_length(w) > 0) {
      if (jcrd_add(j, w, k) != 0) {
        goto error_capacity;
      }
      word_reinit(w);
    }
    fclose(f);
  }
  int n = (jcrd_get_nb_files(j) * (jcrd_get_nb_files(j) - 1)) / 2;
  for (int k = 0; k < n; ++k) {
    printf(" - %zu", jcrd_get_inter(j)[k]);
  }
  printf("\n");
  size_t *card = jcrd_get_cardinals(j);
  int nb_files = jcrd_get_nb_files(j);
  size_t *inter = jcrd_get_inter(j);
  size_t idx = 0;
  for (int i = 0; i < nb_files ; i++) {
    for (int j = i + 1; j < nb_files; j++) {
      size_t intersection = inter[idx];
      size_t union_ = card[i] + card[j] - intersection;
      double distance = 1.0 - (double) intersection / (double) union_;
      printf("distance de jacc pour %d et %d: %.4f\n", i, j, distance);
      ++idx;
    }
  }
  goto dispose;
error_capacity:
  fprintf(stderr, ALLOC_ERROR);
  r = EXIT_FAILURE;
dispose:
  word_dispose(&w);
  jcrd_dispose(&j);
opt_dispose:
  opt_dispose(&option);
  return r;
}
