// word.h
#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdlib.h>
#include <stdbool.h>

//  struct word : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un mot de longueur indéfinie
typedef struct word word;

// word_init : tente d'allouer les ressources necessaires pour gérer un nouveau
// mot initialement vide. Renvoie nullptr en cas d'erreur d'allocation , renvoie
// sinon le pointeur vers le controleur de ce nouveau mot
word *word_init();

//  word_add : tente d'inserer le caractere c au mot associé à w . Renvoie
// nullptr en cas de depassement de capacité , renvoie sinon le pointeur vers le
// nouveau mot
void *word_add(word *w, int c);

//word_reinit : transforme le mot associé à w en mot vide et met sa longueur à
// zero
void word_reinit(word *w);

//word_is_empty: renvoie true si le mot associé à w est vide , faux sinon
bool word_is_empty(word *w);

//word_get: renvoie la chaine de caractere associée au mot w
char *word_get(word *w);

//word_length: renvoie la longueur du mot associé à w
size_t word_length(word *w);

//  word_dispose : sans effet si *wptr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de l'element associé à
//    *wptr puis affecte un pointeur nul à *wptr.
void word_dispose(word **wptr);

#endif
