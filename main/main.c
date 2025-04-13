#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opt.h"
#include "jaccard.h"

#define ALLOC_ERROR "An allocation error occurred\n"
#define FERROR "An error occurred while treating file: %s\n"
#define ERROR_ -1

#define EXEC "test"
#define WRNG_CUT_MSG EXEC ": Word from file '%s' cut: '%s...'.\n"

int main(int argc, char *argv[]) {
  int r = EXIT_SUCCESS;
  opt *option = opt_empty();
  if (option == nullptr) {
    fprintf(stderr, ALLOC_ERROR);
    return EXIT_FAILURE;
  }
  opt_create(option, argv, argc);
  jcrd *j = jcrd_init(opt_get_files(option), opt_get_nb_files(option), true);
  if (j == nullptr) {
    fprintf(stderr, ALLOC_ERROR);
    r = EXIT_FAILURE;
    goto opt_dispose;
  }
  word *w = word_init();
  if (w == nullptr) {
    fprintf(stderr, ALLOC_ERROR);
    r = EXIT_FAILURE;
    goto jcrd_dispose;
  }
  //printf("%d\n", jcrd_get_nb_files(j));
  //for (int k = 0; k < jcrd_get_nb_files(j); ++k) {
    //fprintf(stderr, "Processing file: %s\n", jcrd_get_inputs_name(j)[k]);
  //}
  for (int k = 0; k < jcrd_get_nb_files(j); ++k) {
    FILE *f = nullptr;
    const char *filename = opt_get_files(option)[k];
    if (strcmp(filename, STDIN_FILE) == 0) {
      f = stdin;
      fprintf(stderr, "Reading from standard input\n");
    } else {
      f = fopen(filename, "r");
      if (f == nullptr) {
        fprintf(stderr, FERROR, filename);
        r = ERROR_;
        goto word_dispose;
      }
      //fprintf(stderr, "Opened file: %s\n", filename);
    }
    int c;
    int len = 0;
    int max_len = opt_get_word_max_lenght(option);
    int (*is_blank)(int) = opt_get_is_blank(option);
    while ((c = fgetc(f)) != EOF) {
      bool end_of_word = false;
      if (is_blank(c)) {
        end_of_word = true;
      } else {
        if (word_add(w, c) == nullptr) {
          fprintf(stderr, ALLOC_ERROR);
          r = ERROR_;
          if (f != stdin) {
            fclose(f);
          }
          goto word_dispose;
        }
        len++;
        if (max_len > 0 && len == max_len) {
          end_of_word = true;
        }
      }
      if (end_of_word && word_length(w) > 0) {
        //printf("Mot créé : %s\n", word_get(w));
        if (jcrd_add(j, w, k) != 0) {
          fprintf(stderr, ALLOC_ERROR);
          r = ERROR_;
          if (f != stdin) {
            fclose(f);
          }
          goto word_dispose;
        }
        if (max_len > 0 && len == max_len) {
          int next = fgetc(f);
          if (next != EOF && !is_blank(next)) {
            fprintf(stderr, WRNG_CUT_MSG, filename, word_get(w));
            ungetc(next, f);
          }
        }
        word_reinit(w);
        len = 0;
      }
    }
    if (word_length(w) > 0) {
      //printf("Mot créé : %s\n", word_get(w));
      if (jcrd_add(j, w, k) != 0) {
        fprintf(stderr, ALLOC_ERROR);
        r = ERROR_;
        if (f != stdin) {
          fclose(f);
        }
        goto word_dispose;
      }
      word_reinit(w);
    }
    if (f != stdin && f != NULL) {
      fclose(f);
    }
  }
  //int n = (jcrd_get_nb_files(j) * (jcrd_get_nb_files(j) - 1)) / 2;
  //printf("Intersections:");
  //for (int k = 0; k < n; ++k) {
    //printf(" %zu ", jcrd_get_inter(j)[k]);
  //}
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
      printf("%.4f %s %s\n",
      distance,
          jcrd_get_inputs_name(j)[i],
          jcrd_get_inputs_name(j)[j2]);
      ++idx;
    }
  }
  //jcrd_print_graph(j);
word_dispose:
  word_dispose(&w);
jcrd_dispose:
  jcrd_dispose(&j);
opt_dispose:
  opt_dispose(&option);
  return r;
}
