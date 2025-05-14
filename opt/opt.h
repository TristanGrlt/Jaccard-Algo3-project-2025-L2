// opt : Module permettant la gestion les options de la ligne de commande en
// mémorisant leur contenue dans un trouseau d'option.

//  Un trouseau d'options est une structure qui aspire à contenir les options
//    lue sur la ligne de commande une fois qu'elle on été annalysé.

//  Choix d'implantation
//    On majore le nombre de fichier pouvant être spécifiait sur la sortie
//    standard par la macro constante MAX_FILE. Cette macro constante est fixé à
//    64 par defaut.

#ifndef OPT_H
#define OPT_H

#include <stddef.h>
#include <stdlib.h>

#define VERSION "0.1"

#define LICENCE \
  "This is freeware: you can redistribute it. There is NO WARRANTY."

#define MAX_FILE 64

#define STDIN ""
#define STDIN_FILE "\"\""
#define WORD_MAX_DEFAULT 0

#define EXE(n) (strrchr((n)[0],                                                \
      '/') != nullptr ? strrchr((n)[0], '/') + 1 : (n)[0])

//  struct opt, opt : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un trouseau d'options.
typedef struct opt opt;

//  opt_empty : tente d'allouer les ressources nécessaires pour gérer un nouveau
//    trousseau d'option initilisé à sa valeur par defaut. Renvoie un pointeur
//    nul en cas de dépassement de capacité. Renvoie sinon un pointeur vers le
//    contrôleur associé au trouseau d'options.
opt *opt_empty(void);

//  opt_dispose : sans effet si *optr vaut un pointeur nul. Libère sinon
//    les ressources allouées la gestion du trouseau d'options associé à *opt
//    puis affecte un pointeur nul à *optr.
void opt_dispose(opt **optr);

// opt_create : tente de de remplire le trouseau d'options avec les
//    arguments contenue dans le tableau argv[] de longeur argc. Renvoit un
//    nombre strictement négatif en cas d'erreur, strictement positive dans le
//    cas d'options blocante lue, une valeur nulle sinon.
int opt_create(opt *p, char *argv[], int argc);

// opt_get_files : Retourne le tableau des noms de fichiers.
extern const char **opt_get_files(opt *o);

// opt_get_nb_files : Retourne le nombre de fichiers.
extern int opt_get_nb_files(opt *o);

// opt_get_word_max_lenght : Retourne la longueur maximale d’un mot autorisée.
extern int opt_get_word_max_lenght(opt *o);

// opt_get_is_blank : Retourne un pointeur vers la fonction identifiant les
//    séparateurs.
extern int (*opt_get_is_blank(opt * o))(int);

// opt_get_graph_print : Retourne true ou false cellon que l'affichage du graph
//    d'apartenance des mots est demandé par l'utilisateur
extern bool opt_get_graph_print(opt *o);

#include "opt_ip.h"
#endif
