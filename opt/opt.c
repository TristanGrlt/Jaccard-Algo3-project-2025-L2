// partie implantation du module opt.

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "opt.h"
#include "stack.h"

#define USAGE "Usage: [options]... [file]...\n"

// All the available options. Options needing an argument are suffixed by a
// colon.
#define OPT_HELP "-?"
#define USAGE_HELP "  -?, Display this help and exit\n"

#define OPT_MAX_WORD_LENGTH "-i"
#define USAGE_MAX_WORD_LENGTH \
  "  -i[value], Set the maximum word length value. Defaut to unlimited\n"

#define OPT_GRAPH "-g"
#define USAGE_GRAPH "  -g, Display the graph of the words\n"

#define OPT_PUNC_LIKE_SPACE "-p"
#define USAGE_PUNC_LIKE_SPACE \
  "  -p, Treat punctuation as space if set. Default to false\n"

#define OPT_NEXT_FILE "--"
#define USAGE_NEXT_FILE "  --, Define the next element as file name\n"

#define OPT_STDIN "-"
#define USAGE_STDIN "  -, Read from standard input\n"
#define STDIN ""

struct opt {
  int (*isBlank)(int);
  int world_max_lenght;
  bool graph;
  stack *files;
};

//  isspace_ispunct : Renvoie 1 si le caractère c est un espace ou une
//    ponctuation, 0 sinon.
static int isspace_ispunct(int c) {
  return isspace(c) || ispunct(c);
}

static void opt_print_help() {
  fprintf(stdout,
      USAGE "\n\n"
      "Options:\n"
      "\t" USAGE_HELP
      "\t" USAGE_MAX_WORD_LENGTH
      "\t" USAGE_GRAPH
      "\t" USAGE_PUNC_LIKE_SPACE
      "\t" USAGE_NEXT_FILE
      "\t" USAGE_STDIN
      "\n"
      );
}

opt *opt_empty(void) {
  opt *p = malloc(sizeof(*p));
  if (p == nullptr) {
    return nullptr;
  }
  p->isBlank = isspace;
  p->world_max_lenght = -1;
  p->graph = false;
  p->files = stack_empty();
  if (p->files == nullptr) {
    free(p);
    return nullptr;
  }
  return p;
}

void opt_dispose(opt **pp) {
  if (*pp == nullptr) {
    return;
  }
  stack_dispose(&(*pp)->files);
  free(*pp);
  *pp = nullptr;
}

int opt_create(opt *p, char *argv[], int argc) {
  int k = 1;
  while (k < argc) {
    if (strcmp(argv[k], OPT_HELP) == 0) {
      opt_print_help();
    } else if (strcmp(argv[k], OPT_GRAPH) == 0) {
      printf("graphe\n");
      p->graph = true;
    } else if (strcmp(argv[k], OPT_PUNC_LIKE_SPACE) == 0) {
      printf("no punct\n");
      p->isBlank = isspace_ispunct;
    } else if (strncmp(argv[k], OPT_MAX_WORD_LENGTH, strlen(
        OPT_MAX_WORD_LENGTH)) == 0) {
      if (strlen(argv[k]) <= strlen(OPT_MAX_WORD_LENGTH)) {
        printf("error on max world length, no number specified\n");
        return -1;
      }
      char *s = argv[k] + strlen(OPT_MAX_WORD_LENGTH);
      char *r;
      long m = strtol(s, &r, 10);
      if (*r != '\0' || m > INT_MAX) {
        printf("error on max world length, not a number or number too big\n");
        return -1;
      }
      p->world_max_lenght = (int) m;
      printf("-i%d\n", p->world_max_lenght);
    } else if (strcmp(argv[k], OPT_NEXT_FILE) == 0) {
      if (k + 1 >= argc) {
        printf("error on next is file, file name not specified after\n");
        return -1;
      }
      if (stack_push(p->files, &argv[k + 1]) == nullptr) {
        printf("no more space\n");
        return -1;
      }
      printf("added the file : %s\n", argv[k + 1]);
      k += 1;
    } else if (strcmp(argv[k], OPT_STDIN) == 0) {
      stack_push(p->files, STDIN);
      printf("added standard input as a file\n");
    } else {
      if (stack_push(p->files, &argv[k]) == nullptr) {
        printf("no more space\n");
        return -1;
      }
      printf("added the file : %s\n", argv[k]);
    }
    k += 1;
  }
  return 0;
}
