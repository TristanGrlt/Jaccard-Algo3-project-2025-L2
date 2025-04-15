// opt : Module permettant la gestion les options de la ligne de commande en
// mémorisant leur contenue dans une structure nommée opt.

#ifndef OPT_H
#define OPT_H

#include <stddef.h>
#include <stdlib.h>


#define VERSION "0.1"

#define MAX_FILE 64

#define STDIN ""
#define OPT_STDIN "-"
#define STDIN_FILE "\"\""
#define WORD_MAX_DEFAULT 0

typedef struct opt opt;

// opt_empty : tente d'allouer les ressources nécessaires pour gérer une nouveau
// gestionnaire d'option initialisé avec les champs initilisé à leur valeur de
// départ. Renvoie un pointeur nul en cas de dépassement de capacité. Renvoie
// sinon un pointeur vers le contrôleur associé
// à l'option.
opt *opt_empty(void);

// opt_dispose : sans effet si *optr est un pointeur nul. Libère sinon les
// ressources allouées à la gestion de l'option associée à *optr puis affecte
// nul à *optr.
void opt_dispose(opt **optr);

// opt_create : tente de de remplire le gestionnaire d'options avec les
// arguments contenue dans le tableau argv[] de longeur argc. Renvoit un nombre
// strictement négatif en cas d'erreur de tout type une valeur nulle sinon.
int opt_create(opt *p, char *argv[], int argc);

extern const char **opt_get_files(opt *o);

extern int opt_get_nb_files(opt *o);

extern int opt_get_word_max_lenght(opt *o);

extern int (*opt_get_is_blank(opt *o))(int);

extern bool opt_get_graph_print(opt *o);
#endif
