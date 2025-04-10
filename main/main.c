#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "opt.h"
#include "jaccard.h"

int main(int argc, char *argv[]) {
  opt *option = opt_empty();
  if (option == nullptr) {
    printf("error no more place");
    return EXIT_SUCCESS;
  }
  opt_create(option, argv, argc);
  jcrd *j = jcrd_init(opt_get_files(option), false);
  if (j == nullptr) {
    return 0;
  }
  word *w = word_init();
  printf("%zu\n", jcrd_get_nb_files(j));
  for (size_t k = 0; k < jcrd_get_nb_files(j); ++k) {
    FILE *f = fopen(jcrd_get_inputs_name(j)[k], "r");
    if (f == nullptr) {
      return 0;
    }
    int c;
    while ((c = fgetc(f)) != EOF) {
      if (isspace(c)) {
        element *e = element_init(w, jcrd_get_nb_files(j), k);
        jcrd_add(j, e, k);
        word_reinit(w);
        //while(isspace(fgetc(f))){
        //}
      } else {
        word_add(w, c);
      }
    }
    if (word_length(w) > 0) {
      element *e = element_init(w, jcrd_get_nb_files(j), k);
      jcrd_add(j, e, k);
      word_reinit(w);
    }
    fclose(f);
  }
  size_t n = (jcrd_get_nb_files(j) * (jcrd_get_nb_files(j) - 1)) / 2;
  for (size_t k = 0; k < n; ++k) {
    printf(" - %zu", jcrd_get_inter(j)[k]);
  }
  opt_dispose(&option);
  return EXIT_SUCCESS;
}
