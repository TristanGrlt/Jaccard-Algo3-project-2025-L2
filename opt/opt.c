// partie implantation du module opt.

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "opt.h"

//---- [ERROR MESSAGE DISPLAY] -----------------------------------------------//
//----------------------------------------------------------------------------//

#define ERROR_MESSAGE_ARG(exe, msg, arg)                                       \
  {                                                                            \
    fprintf(stderr, "%s: %s \'" OPT "%s\'\n", exe, msg, arg);                  \
    HELP_MORE_INFO(exe)                                                        \
  }

#define ERROR_MESSAGE(exe, msg)                                                \
  {                                                                            \
    fprintf(stderr, "%s : %s.\n", exe, msg);                                   \
    HELP_MORE_INFO(exe)                                                        \
  }

#define HELP_MORE_INFO(exe)                                                    \
  {                                                                            \
    fprintf(stderr, "Try \'%s " OPT OPT_HELP "\' for more information.\n", exe); \
  }

//---- [MESSAGE] -------------------------------------------------------------//
//----------------------------------------------------------------------------//

#define USAGE "Usage: %s [OPTION]... FILE1 FILE2 [FILE]...\n"

#define MISSING_OPERAND "Missing operand"

#define INVALIDE_ARGUMENT "Invalid argument"

#define MISSING_FILE "Missing filename after '" OPT_NEXT_FILE "'"

#define OUT_OF_MEMORIE                                                         \
  "Cannot alocated the ressources for the gestion of this execution"

//---- [TOOLS] ---------------------------------------------------------------//
//----------------------------------------------------------------------------//

// is_strict_prefix : renvoit true ou false selon que la cahine de caractères
// pointé par s1 soit un préfixe stricte de la cahine de caractères pointé par
// s2
static bool is_strict_prefix(const char *s1, const char *s2) {
  size_t k = 0;
  while (s1[k] != '\0' && s2[k] != '\0') {
    if (s1[k] != s2[k]) {
      return false;
    }
    ++k;
  }
  return s1[k] == '\0' && s2[k] != '\0';
}

//---- [USAGE] ---------------------------------------------------------------//
//----------------------------------------------------------------------------//
#define DESC                                                                   \
  "Computes Jaccard dissimilarities of sets of words in FILEs."                \
  "\n\n"                                                                       \
  "For any pair of FILEs, dissimilarity is displayed first to four decimal "   \
  "places,\nfollowed by the two FILEs in the pair. A word is, by default, a "  \
  "maximum length\nsequence of characters that do not belong to the "          \
  "white-space characters set."                                                \
  "\n\n"                                                                       \
  "Read the standard input for any FILE that is '-' on command line. The "     \
  "standard\ninput is displayed as a pair of double quotation marks in "       \
  "productions."                                                               \
  "\n\n"

#define USAGE_HELP "\t\tPrint this help message and exit.\n"

#define USAGE_MAX_WORD_LENGTH                                                  \
  "\t\tSet the maximal number of significant initial letters for words\n"      \
  "\t\tto VALUE. 0 means without limitation. Default is 0.\n"

#define USAGE_GRAPH                                                            \
  "\t\tSuppress normal output. Instead, for each word found in any FILE\n"     \
  "\t\tjdis list the FILEs in which it does or does not appear. A \n"          \
  "\t\theader line indicates the FILE names: the name of the first FILE\n"     \
  "\t\tappears in the second column, that of the second in the third,\n"       \
  "\t\tand so on. For the subsequent lines, a word appears in the first\n"     \
  "\t\tcolumn, followed by appearance marks: 'x' for yes, '-' for no.\n"       \
  "\t\tThe list is lexicographically sorted. The locale specified by\n"        \
  "\t\tthe environment affects the sort order. Set 'LC_ALL=C' or\n"            \
  "\t\t'LC_COLLATE=C' to get the traditional sort order that uses\n"           \
  "\t\tnative byte values.s\n"

#define USAGE_PUNC_LIKE_SPACE                                                  \
  "\t\tMake the punctuation characters play the same role as\n"                \
  "\t\twhite-space characters in the meaning of words.\n"

#define USAGE_USAGE                                                            \
  "\t\tPrint a short usage message and exit.\n"

#define USAGE_VERSION                                                          \
  "\t\tPrint version information.\n"

//---- [OPTION] --------------------------------------------------------------//
//----------------------------------------------------------------------------//

#define OPT "-"

#define OPT_HELP "?"

#define OPT_USAGE "u"

#define OPT_VERSION "v"

#define OPT_MAX_WORD_LENGTH "i"

#define OPT_GRAPH "g"

#define OPT_PUNC_LIKE_SPACE "p"

#define OPT_NEXT_FILE "--"

#define OPT_STDIN "-"

//---- [STRUCTUR] ------------------------------------------------------------//
//----------------------------------------------------------------------------//

struct opt {
  int (*isBlank)(int);
  int word_max_lenght;
  bool graph;
  char const **files;
  int nb_files;
};

//---- [CODE] ----------------------------------------------------------------//
//----------------------------------------------------------------------------//

//  isspace_ispunct : Renvoie 1 si le caractère c est un espace ou une
//    ponctuation, 0 sinon.
static int isspace_ispunct(int c) {
  return isspace(c) || ispunct(c);
}

static void opt_print_help(char **exe) {
  fprintf(stdout,
      USAGE
      "\n" DESC
      "Program Information\n"
      "\t" OPT OPT_HELP "\n" USAGE_HELP
      "\n"
      "\t" OPT OPT_USAGE "\n" USAGE_USAGE
      "\n"
      "\t" OPT OPT_VERSION "\n" USAGE_VERSION
      "\n"
      "Input Control\n"
      "\t" OPT OPT_MAX_WORD_LENGTH "VALUE"
      "\n" USAGE_MAX_WORD_LENGTH
      "\n"
      "\t" OPT OPT_PUNC_LIKE_SPACE "\n" USAGE_PUNC_LIKE_SPACE
      "\n"
      "Output Control\n"
      "\t" OPT OPT_GRAPH "\n" USAGE_GRAPH
      "\nWhite-space and punctuation characters conform to the standard. At"
      "most 64 FILEs\nare supported.\n",
      EXE(exe));
}

opt *opt_empty(void) {
  opt *p = malloc(sizeof(*p));
  if (p == nullptr) {
    return nullptr;
  }
  p->isBlank = isspace;
  p->word_max_lenght = 0;
  p->graph = false;
  p->files = malloc(MAX_FILE);
  p->nb_files = 0;
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
  free((*pp)->files);
  free(*pp);
  *pp = nullptr;
}

int opt_create(opt *p, char *argv[], int argc) {
  int k = 1;
  int status = 0;
  while (k < argc) {
    // Next argument is a file
    if (strcmp(argv[k], OPT_NEXT_FILE) == 0) {
      if (k + 1 >= argc) {
        ERROR_MESSAGE(EXE(argv), MISSING_FILE);
        return -1;
      }
      if (p->nb_files == MAX_FILE) {
        ERROR_MESSAGE_ARG(EXE(argv), OUT_OF_MEMORIE, argv[k]);
        return -1;
      }
      p->files[p->nb_files] = argv[k + 1];
      p->nb_files += 1;
      k += 1;
      //Argument is standard input
    } else if (strcmp(argv[k], OPT_STDIN) == 0) {
      if (p->nb_files == MAX_FILE) {
        ERROR_MESSAGE_ARG(EXE(argv), OUT_OF_MEMORIE, argv[k]);
        return -1;
      }
      p->files[p->nb_files] = STDIN;
      p->nb_files += 1;
      //Argument is an option
    } else if (is_strict_prefix(OPT, argv[k])) {
      argv[k] += strlen(OPT);
      // It is help
      if (strcmp(argv[k], OPT_HELP) == 0) {
        status = -1;
        // It is usage
      } else if (strcmp(argv[k], OPT_USAGE) == 0) {
        status = 1;
        // It is version
      } else if (strcmp(argv[k], OPT_VERSION) == 0) {
        status = 2;
        // It is graph
      } else if (strcmp(argv[k], OPT_GRAPH) == 0) {
        p->graph = true;
        // It is punct
      } else if (strcmp(argv[k], OPT_PUNC_LIKE_SPACE) == 0) {
        p->isBlank = isspace_ispunct;
        // It is limit of char
      } else if (strncmp(argv[k], OPT_MAX_WORD_LENGTH, strlen(
            OPT_MAX_WORD_LENGTH)) == 0) {
        if (strlen(argv[k]) <= strlen(OPT_MAX_WORD_LENGTH)) {
          ERROR_MESSAGE_ARG(EXE(argv), INVALIDE_ARGUMENT, argv[k]);
          return -1;
        }
        char *s = argv[k] + strlen(OPT_MAX_WORD_LENGTH);
        char *r;
        long m = strtol(s, &r, 10);
        if (*r != '\0' || m > INT_MAX || m < 0) {
          ERROR_MESSAGE_ARG(EXE(argv), INVALIDE_ARGUMENT, argv[k]);
          return -1;
        }
        p->word_max_lenght = (int) m;
      } else {
        ERROR_MESSAGE_ARG(EXE(argv), INVALIDE_ARGUMENT, argv[k]);
        return -1;
      }
    } else {
      if (p->nb_files == MAX_FILE) {
        ERROR_MESSAGE_ARG(EXE(argv), OUT_OF_MEMORIE, argv[k]);
        return -1;
      }
      p->files[p->nb_files] = argv[k];
      p->nb_files += 1;
    }
    k += 1;
  }
  if (status == -1) {
    opt_print_help(argv);
    return 1;
  } else if (status == 1) {
    fprintf(stderr, USAGE "\n", EXE(argv));
    return 1;
  } else if (status == 2) {
    fprintf(stderr, "%s - " VERSION "\n", EXE(argv));
    fprintf(stderr, LICENCE "\n");
    return 1;
  }
  if (p->nb_files < 2) {
    ERROR_MESSAGE(EXE(argv), MISSING_OPERAND);
    return -1;
  }
  return 0;
}

const char **opt_get_files(opt *o) {
  return o->files;
}

int opt_get_nb_files(opt *o) {
  return o->nb_files;
}

int opt_get_word_max_lenght(opt *o) {
  return o->word_max_lenght;
}

int (*opt_get_is_blank(opt * o))(int)
{
  return o->isBlank;
}

bool opt_get_graph_print(opt *o) {
  return o->graph;
}
