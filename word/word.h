// word : Module permettant la gestion de mots de longeur indéfinie

//  Un mot est une structure qui contient une chaine de carractère associé à sa
//    taille ainsi que la longeur du mot

//  Choix d'implantation
//    A tout face stable de l'éxécution de ce programme, mot représente une
//    chaine de carractere valide

#ifndef WORD_H
#define WORD_H

#include <stdlib.h>
#include <stdbool.h>

//  struct word : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un mot de longueur indéfinie
typedef struct word word;

//  word_init : tente d'allouer les ressources necessaires pour gérer un nouveau
//    mot initialement vide. Renvoie nullptr en cas d'erreur d'allocation,
//    renvoie sinon le pointeur vers le controleur associé au nouveau mot
word *word_init();

//  word_dispose : sans effet si *wptr vaut un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de l'element associé à *wptr puis affecte
//    un pointeur nul à *wptr.
void word_dispose(word **wptr);

//  word_add : tente d'inserer le caractere c à la fin du mot associé à w.
//    Renvoie nullptr en cas de depassement de capacité, renvoie sinon le
//    pointeur vers le nouveau mot.
void *word_add(word *w, int c);

//  word_reinit : réinitilase le mot associé à w en mot vide
void word_reinit(word *w);

//  word_is_empty: renvoie true si le mot associé à w est vide, faux sinon
bool word_is_empty(word *w);

//  word_get: renvoie la chaine de caractere associée au mot w
char *word_get(const word *w);

//  word_get_clean: tente de recopier le mot associé à w dans la zonne
//    mémoire pointé par dest en supposant cette zonne de taille suffisante.
void word_get_clean(const word *w, char *dest);

//  word_length: renvoie la longueur du mot associé à w
size_t word_length(const word *w);
#endif
