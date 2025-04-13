#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "opt.h"
#include "jaccard.h"

#define ALLOC_ERROR "An allocation error occurred"
#define FERROR "An error occurred while treating file: "
#define ERROR_ -1

#define EXEC "jcrd"
#define WRNG_CUT_MSG EXEC ": Word from file '%s' cut: '%s...'.\n"

int main(int argc, char *argv[]) {
  int r = EXIT_SUCCESS;
  opt *option = opt_empty();
  if (option == NULL) {
    r = EXIT_FAILURE;
    goto opt_dispose;
  }
  opt_create(option, argv, argc);
  jcrd *j = jcrd_init(opt_get_files(option), opt_get_nb_files(option), true);
  if (j == NULL) {
    goto error_capacity;
  }
  word *w = word_init();
  if (w == NULL) {
    goto error_capacity;
  }
  printf("%d\n", jcrd_get_nb_files(j));
  for (int k = 0; k < jcrd_get_nb_files(j); ++k) {
    fprintf(stderr, "%s\n", jcrd_get_inputs_name(j)[k]);
  }
  for (int k = 0; k < jcrd_get_nb_files(j); ++k) {
    FILE *f = fopen(jcrd_get_inputs_name(j)[k], "r");
    if (f == NULL) {
      fprintf(stderr, FERROR "%s\n", jcrd_get_inputs_name(j)[k]);
      r = ERROR_;
      goto dispose;
    }
    int c;
    int len = 0;
    int max_len = opt_get_world_max_lenght(option);
    int (*is_blank)(int) = opt_get_is_blank(option);
    while ((c = fgetc(f)) != EOF) {
      bool end_of_word = false;
      if (is_blank(c)) {
        end_of_word = true;
      } else {
        if (word_add(w, c) == NULL) {
          goto error_capacity;
        }
        len++;
        if (max_len > 0 && len == max_len) {
          end_of_word = true;
        }
      }
      if (end_of_word && word_length(w) > 0) {
        printf("Mot créé : %s \n", word_get(w));
        if (jcrd_add(j, w, k) != 0) {
          goto error_capacity;
        }
        if (max_len > 0 && len == max_len) {
          int next = fgetc(f);
          if (next != EOF && !is_blank(next)) {
            fprintf(stderr, WRNG_CUT_MSG, opt_get_files(option)[k], word_get(w));
            ungetc(next, f);
          }
        }
        word_reinit(w);
        len = 0;
      }
    }
    if (word_length(w) > 0) {
      printf("Mot créé : %s\n", word_get(w));
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
  for (int i = 0; i < nb_files; i++) {
    for (int j2 = i + 1; j2 < nb_files; j2++) {
      size_t intersection = inter[idx];
      size_t union_ = card[i] + card[j2] - intersection;
      double distance = 1.0 - (double) intersection / (double) union_;
      printf("distance de jacc pour %d et %d: %.4f\n", i, j2, distance);
      ++idx;
    }
  }
  jcrd_print_graph(j);
  goto dispose;
error_capacity:
  fprintf(stderr, ALLOC_ERROR);
dispose:
  r = EXIT_FAILURE;
  word_dispose(&w);
  jcrd_dispose(&j);
opt_dispose:
  opt_dispose(&option);
  return r;
}
