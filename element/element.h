// element.h
#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdlib.h>
#include <stdbool.h>

//  struct element : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un nouvel element
typedef struct element element;

//  element_init : tente d'allouer les ressources nécessaires pour gérer un
// nouvel element de chaine le mot associé à w et alloue un tableau de taille
// numFiles initialisé à false pour toutes les cases, true à celle d'indice
// file_index .
element *element_init(const word *w, int numFiles, int file_index);

//  element_dispose : sans effet si *eptr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de l'element associé à
//    *eptr puis affecte un pointeur nul à *eptr.
void element_dispose(element **eptr);

//  element_compar: Renvoie une fonction de tri sur les deux champs s de e1 et
// e2. (tri lexicographique).
int element_compar(const element *e1, const element *e2);

#endif
