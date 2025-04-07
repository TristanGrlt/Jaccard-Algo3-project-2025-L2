// partie implantation du module opt.

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "opt.h"
#include "stack.h"

#define STR(s)  #s
#define XSTR(s) STR(s)

#define ERROR_MESSAGE(exe, msg, arg) {                                         \
    fprintf(stderr, "%s : %s. Here : \'%s\'\n", exe, msg, arg);                \
}

#define USAGE "Usage: %s [options]... [file]...\n"

#define DESC                                                                   \
  "\tJaccard-based similarity. Compute and display Jaccard distances between\n"  \
  "\tsets of words extracted from the given text FILES.\n\n"                     \
  "\tA word is, by default, a sequence of letters and digits, delimited by\n"    \
  "\tany non-alphanumeric character. The maximum word length can be limited\n"    \
  "\tusing the '-iN' option.\n\n"                                                \
  "\tWhen two or more FILES are provided, the program computes the pairwise\n"   \
  "\tJaccard distance between each pair of files, based on their word sets.\n"\
  "\tThe Jaccard distance is defined as one minus the ratio of the size of\n"\
  "\tthe intersection to the size of the union of the two sets.\n\n"                 \
  "\tWhen the '-g' option is specified, the program displays a graph of\n"       \
  "\tfile-word memberships. Each word is written and a \'x\' or a \'-\' is\n"     \
  "\tdisplay regarding theyr presence in the associted file\n\n"                 \
  "\tRead the standard input when no FILE is given or for any FILE which is\n"   \
  "\t \"-\". In such cases, \"\" is displayed as the name associated\n"\
  "\twith the FILE.\n\n"

#define OPT_HELP "-?"
#define USAGE_HELP "  -?, Display this help and exit\n"

#define OPT_MAX_WORD_LENGTH "-i"
#define USAGE_MAX_WORD_LENGTH \
  "  -i[value], Set the maximum word length value. Defaut to unlimited\n"
#define ERROR_MAX_WORD_LENGTH \
  "The argument must be a positive value >0 and less than MAX_INT"

#define OPT_GRAPH "-g"
#define USAGE_GRAPH "  -g, Display the graph of the words\n"

#define OPT_PUNC_LIKE_SPACE "-p"
#define USAGE_PUNC_LIKE_SPACE \
  "  -p, Treat punctuation as space if set. Default to false\n"

#define OPT_NEXT_FILE "--"
#define USAGE_NEXT_FILE "  --, Define the next element as file name\n"
#define ERROR_NEXT_FILE "The next element must be a file but it is not given"

#define OPT_STDIN "-"
#define USAGE_STDIN "  -, Read from standard input\n"
#define STDIN ""

#define OUT_OF_MEMORIE \
  "Cannot alocated the ressources for the gestion of this execution"

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

static void opt_print_help(char *exe) {
  fprintf(stdout,
      USAGE "\n\n"
      "DESCRIPTION :\n"
      DESC
      "OPTIONS :\n"
      "\t" USAGE_HELP
      "\t" USAGE_MAX_WORD_LENGTH
      "\t" USAGE_GRAPH
      "\t" USAGE_PUNC_LIKE_SPACE
      "\t" USAGE_NEXT_FILE
      "\t" USAGE_STDIN
      "\n", exe
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
      opt_print_help(argv[0]);
    } else if (strcmp(argv[k], OPT_GRAPH) == 0) {
      printf("graphe\n");
      p->graph = true;
    } else if (strcmp(argv[k], OPT_PUNC_LIKE_SPACE) == 0) {
      printf("no punct\n");
      p->isBlank = isspace_ispunct;
    } else if (strncmp(argv[k], OPT_MAX_WORD_LENGTH, strlen(
        OPT_MAX_WORD_LENGTH)) == 0) {
      if (strlen(argv[k]) <= strlen(OPT_MAX_WORD_LENGTH)) {
        ERROR_MESSAGE(argv[0], ERROR_MAX_WORD_LENGTH, argv[k]);
        return -1;
      }
      char *s = argv[k] + strlen(OPT_MAX_WORD_LENGTH);
      char *r;
      long m = strtol(s, &r, 10);
      if (*r != '\0' || m > INT_MAX || m <= 0) {
        ERROR_MESSAGE(argv[0], ERROR_MAX_WORD_LENGTH, argv[k]);
        return -1;
      }
      p->world_max_lenght = (int) m;
      printf("-i%d\n", p->world_max_lenght);
    } else if (strcmp(argv[k], OPT_NEXT_FILE) == 0) {
      if (k + 1 >= argc) {
        ERROR_MESSAGE(argv[0], ERROR_NEXT_FILE, argv[k]);
        return -1;
      }
      if (stack_push(p->files, argv[k + 1]) == nullptr) {
        ERROR_MESSAGE(argv[0], OUT_OF_MEMORIE, argv[k]);
        return -1;
      }
      printf("added the file : %s\n", argv[k + 1]);
      k += 1;
    } else if (strcmp(argv[k], OPT_STDIN) == 0) {
      stack_push(p->files, STDIN);
      printf("added standard input as a file\n");
    } else {
      if (stack_push(p->files, argv[k]) == nullptr) {
        ERROR_MESSAGE(argv[0], OUT_OF_MEMORIE, argv[k]);
        return -1;
      }
      printf("added the file : %s\n", argv[k]);
    }
    k += 1;
  }
  while (stack_height(p->files) < 2) {
    if (stack_push(p->files, STDIN) == nullptr) {
      ERROR_MESSAGE(argv[0], OUT_OF_MEMORIE, argv[k]);
      return -1;
    }
    printf("added standard input as a file\n");
  }
  return 0;
}
