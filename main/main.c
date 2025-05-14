#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opt.h"
#include "jaccard.h"

#define ALLOC_ERROR "An allocation error occurred\n"
#define FERROR "An error occurred while treating file:"
#define FCLOSE_ERROR "An error occured while closing file:"

#define WRNG_CUT_MSG "%s: Word from file '%s' at line %d cut: '%s...'.\n"

#define START_READ "--- starts reading for "
#define END_READ  "--- ends reading for "

#define FILE_ERROR_MSG(msg, filename) {                                        \
    fprintf(stderr, "%s: %s \'%s\'.\n", EXE(argv), msg, filename);             \
}                                                                              \

#define ERROR_MSG(msg) {                                                       \
    fprintf(stderr, "%s", msg);                                                \
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
    r = EXIT_FAILURE;
    goto opt_dispose;
  }
  if (opt_create(option, argv, argc) != 0) {
    goto opt_dispose;
  }
  bool print_graph = opt_get_graph_print(option);
  jcrd *j = jcrd_init(opt_get_files(option), opt_get_nb_files(
        option), print_graph);
  if (j == nullptr) {
    ERROR_MSG(ALLOC_ERROR);
    r = EXIT_FAILURE;
    goto jcrd_dispose;
  }
  word *w = word_init();
  if (w == nullptr) {
    fprintf(stderr, ALLOC_ERROR);
    goto error;
  }
  for (int k = 0; k < opt_get_nb_files(option); ++k) {
    FILE *f = nullptr;
    bool is_stdin = false;
    const char *filename = opt_get_files(option)[k];
    if (strcmp(filename, STDIN) == 0) {
      f = stdin;
      is_stdin = true;
      START_READING_STDIN(k);
    } else {
      f = fopen(filename, "r");
      if (f == nullptr) {
        FILE_ERROR_MSG(FERROR, filename);
        goto error;
      }
    }
    int c;
    int file_line = 1;
    int max_len = opt_get_word_max_lenght(option);
    bool max_len_default = max_len == WORD_MAX_DEFAULT;
    int (*is_blank)(int) = opt_get_is_blank(option);
    while ((c = fgetc(f)) != EOF) {
      word_reinit(w);
      int len = 0;
      while (c != EOF && !is_blank(c)
          && (max_len_default || len < max_len)) {
        if (word_add(w, c) == nullptr) {
          goto error;
        }
        ++len;
        c = fgetc(f);
      }
      if (c == '\n') {
        file_line++;
      }
      if (!max_len_default
          && (c != EOF && !is_blank(c))) {
        fprintf(stderr, WRNG_CUT_MSG, EXE(argv), filename, file_line, word_get(
              w));
        while ((c = fgetc(f)) != EOF && !is_blank(c)) {
        }
        if (c == '\n') {
          file_line++;
        }
      }
      if (word_length(w) > 0) {
        if (jcrd_add(j, w, k) != 0) {
          ERROR_MSG(ALLOC_ERROR);
          if (f != stdin) {
            if (fclose(f) != 0) {
              FILE_ERROR_MSG(FCLOSE_ERROR, filename);
            }
          }
          goto error;
        }
      }
    }
    if (f != nullptr) {
      if (!feof(f)) {
        FILE_ERROR_MSG(FERROR, filename);
        goto error;
      }
      if (!is_stdin) {
        if (fclose(f) != 0) {
          FILE_ERROR_MSG(FCLOSE_ERROR, filename);
          goto error;
        }
      } else {
        ENDS_READING_STDIN(k);
        rewind(stdin);
      }
    }
  }
  if (!print_graph) {
    jcrd_print_distance(j);
  } else {
    jcrd_print_graph(j);
  }
error:
  r = EXIT_FAILURE;
  word_dispose(&w);
jcrd_dispose:
  jcrd_dispose(&j);
opt_dispose:
  opt_dispose(&option);
  return r;
}
