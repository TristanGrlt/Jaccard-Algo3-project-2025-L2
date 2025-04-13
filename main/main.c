#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opt.h"
#include "jaccard.h"

#define ALLOC_ERROR "An allocation error occurred\n"
#define FERROR "An error occurred while treating file: %s\n"
#define FCLOSE_ERROR "An error occured while closing file: "
#define ERROR_ -1

#define EXEC "test"
#define WRNG_CUT_MSG EXEC ": Word from file '%s' at line %d cut: '%s...'.\n"

#define START_READ "--- starts reading for "
#define END_READ  "--- ends reading for "
#define RESTRICT_FILE "restrict FILE"

#define FILE_ERROR_MSG(msg, filename) {                                        \
fprintf(stderr, EXEC " %s \'%s\'.\n", msg, filename);                          \
}                                                                              \

#define ERROR_MSG(msg) {                                                       \
    fprintf(stderr, "%s" , msg);                                              \
}                                                                              \

#define START_READING_STDIN(num_file) {                                        \
    fprintf(stderr, "\033[30;47m");                                            \
    fprintf(stderr, START_READ);                                               \
    fprintf(stderr, "#%d FILE", num_file + 1);                                 \
    fprintf(stderr, "\033[0m");                                                \
    fprintf(stderr, "\n");                                                     \
}

#define ENDS_READING_STDIN(num_file) {                                         \
    fprintf(stderr, "\033[30;47m");                                            \
    fprintf(stderr, END_READ);                                                 \
    fprintf(stderr, "#%d FILE", num_file + 1);                                 \
    fprintf(stderr, "\033[0m");                                                \
    fprintf(stderr, "\n");                                                     \
}

int main(int argc, char *argv[]) {
  int r = EXIT_SUCCESS;
  opt *option = opt_empty();
  if (option == nullptr) {
      ERROR_MSG(ALLOC_ERROR);
    return EXIT_FAILURE;
  }
  opt_create(option, argv, argc);
  jcrd *j = jcrd_init(opt_get_files(option), opt_get_nb_files(option), true);
  if (j == nullptr) {
    ERROR_MSG(ALLOC_ERROR);
    r = EXIT_FAILURE;
    goto opt_dispose;
  }
  word *w = word_init();
  if (w == nullptr) {
    fprintf(stderr, ALLOC_ERROR);
    r = EXIT_FAILURE;
    goto jcrd_dispose;
  }
  for (int k = 0; k < jcrd_get_nb_files(j); ++k) {
    FILE *f = nullptr;
    bool is_stdin = false;
    const char *filename = opt_get_files(option)[k];
    if (strcmp(filename, STDIN_FILE) == 0) {
      f = stdin;
      is_stdin = true;
      START_READING_STDIN(k);
    } else {
      f = fopen(filename, "r");
      if (f == nullptr) {
        FILE_ERROR_MSG(FERROR, filename);
        r = ERROR_;
        goto word_dispose;
      }
    }
    int c;
    int len = 0;
    int max_len = opt_get_word_max_lenght(option);
    int (*is_blank)(int) = opt_get_is_blank(option);
    int file_line = 1;
    while ((c = fgetc(f)) != EOF) {
      if ( c == '\n') {
        file_line ++;
      }
      bool end_of_word = false;
      if (is_blank(c)) {
        end_of_word = true;
      } else {
        if (word_add(w, c) == nullptr) {
          ERROR_MSG(ALLOC_ERROR);
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
        if (jcrd_add(j, w, k) != 0) {
          ERROR_MSG(ALLOC_ERROR);
          r = ERROR_;
          if (f != stdin) {
            fclose(f);
          }
          goto word_dispose;
        }
        if (max_len > 0 && len == max_len) {
          int next = fgetc(f);
          if (next != EOF && !is_blank(next)) {
            char temp = word_get(w)[max_len];
            word_get(w)[max_len] = '\0';
            fprintf(stderr, WRNG_CUT_MSG, filename, file_line, word_get(w));
            word_get(w)[max_len] = temp;
            while ((next = fgetc(f)) != EOF && !is_blank(next)) {
            }

        }
        if (next != EOF) {
            ungetc(next, f);
        }
      }
        word_reinit(w);
        len = 0;
      }
    }
    if (word_length(w) > 0) {
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
    if (f != nullptr) {
      if (!feof(f)) {
        FILE_ERROR_MSG(FERROR, filename);
        r = ERROR_;
      }
      if (!is_stdin) {
        if (fclose(f) != 0) {
         FILE_ERROR_MSG(FCLOSE_ERROR, filename);
          r = ERROR_;
        }
      } else {
        ENDS_READING_STDIN(k);
        rewind(stdin);
      }
    }
  }
  if (!opt_get_graph_print(option)) {
    size_t *card = jcrd_get_cardinals(j);
    int nb_files = jcrd_get_nb_files(j);
    size_t *inter = jcrd_get_inter(j);
    size_t idx = 0;
    for (int i = 0; i < nb_files; i++) {
      for (int j2 = i + 1; j2 < nb_files; j2++) {
        size_t intersection = inter[idx];
        size_t union_ = card[i] + card[j2] - intersection;
        double distance = 1.0 - (double) intersection / (double) union_;
        printf("%.4f\t%s\t%s\n",
            distance,
            jcrd_get_inputs_name(j)[i],
            jcrd_get_inputs_name(j)[j2]);
        ++idx;
      }
    }
  } else {
    jcrd_print_graph(j);
  }
word_dispose:
  word_dispose(&w);
jcrd_dispose:
  jcrd_dispose(&j);
opt_dispose:
  opt_dispose(&option);
  return r;
}
