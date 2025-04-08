// opt : Module permettant la gestion les options de la ligne de commande en
// mémorisant leur contenue dans une structure nommée opt.

#ifndef OPT_H
#define OPT_H

#include <stddef.h>
#include <stdlib.h>
#include "stack.h"

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

extern stack *opt_get_files(opt *o);



#endif
