// jaccard : Module permetant la réalisation du calcule de la distance de
// Jaccard ainsi que l'affichage d'un graph d'apartenance des mots à leur
// fichiers.

// La réalisation du calcule de la distance de Jaccard ainsi que l'affichage
//  d'un graph d'apartenance des mots à leur fichiers est référé si aprés à
//  simplement calcule de jaccard.

#ifndef JACCARD_H
#define JACCARD_H

#include "word.h"
#include "hashtable.h"

#include <stdio.h>

//  struct jcrd, jcrd : type et nom de type d'un contrôleur regroupant les
//    éléments nécessaires au calcule Jaccard.
typedef struct jcrd jcrd;

//  jcrd_init : tente d'allouer les ressources nécessaires pour gérer un
//    nouveau calcule de Jaccard. Renvoie un pointeur nul en cas de dépassement
//    de capacité. Renvoie sinon un pointeur vers le contrôleur associé au
//    nouveau calcule de Jaccard
extern jcrd *jcrd_init(const char **files, int nb_files, bool graph);

//  jcrd_dispose : sans effet si *jptr est un pointeur nul. Libère sinon les
//    ressources allouées au calule de Jaccard associé à *jptr puis affecte un
//    pointeur nul à *jptr.
extern void jcrd_dispose(jcrd **jptr);

//  jcrd_add : Tente d'ajouter le mot pointé par w au calcule de Jaccard associé
//    à j. renvoit une valeur strictement négative en cas de dépassement de
//    capacité, une valeur null sinon.
extern int jcrd_add(jcrd *j, word *w, int file_index);

//  jcrd_print_graph : tente d'écrire sur la sortie standard le graphe
//    d'appartenance des mots à leur fichiers du calcule de Jaccard associé à j.
//    Les nom des fichiers sont affiché dans le meme ordre que donné par
//    l'utilisateur et l'entré standard est symbolisé par "". Les mots sont
//    ordonné dans l'ordre définie localement par le systhème. Renvoit une
//    valeur strictement négative en cas d'échec, une valeur null en cas de
//    succès.
extern int jcrd_print_graph(jcrd *j);

//  jcrd_print_distance : Affiche sur la sortie standard la distance de Jaccard
//    entre chaque paire de fichiers du calcule de Jaccard associé à j. L'entré
//    standard est symbolisé par "".
extern void jcrd_print_distance(jcrd *j);

//  jcrd_get_nb_files : Retourne le nombre de fichiers
extern int jcrd_get_nb_files(jcrd *j);
#endif
