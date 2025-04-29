// jaccard : Module permetant la réalisation du calcule de la distance de
// Jaccard ainsi que son graphe d'appartennance

#ifndef JACCARD_H
#define JACCARD_H

#include "word.h"
#include "hashtable.h"

#include <stdio.h>

typedef struct jcrd jcrd;

// jcrd_init : tente d'allouer les ressources nécessaires pour gérer un
// controler pour Jaccard Renvoie un pointeur nul en cas de dépassement de
// capacité. Renvoie sinon un pointeur vers le contrôleur associé au controler.
extern jcrd *jcrd_init(const char **files, int nb_files, bool graph);

// jcrd_dispose : sans effet si *jptr est un pointeur nul. Libère sinon les
// ressources allouées à la gestion du controler pour Jaccard associé à *jptr
// puis affecte un pointeur nul à *jptr.
extern void jcrd_dispose(jcrd **jptr);

// jcrd_add : renvoit un pointeur null si e est un pointeur null. Tente sinon
// d'ajouter l'élément pointé par e au controler jaccard pointé par j. renvoit
// un pointeur null en cas de dépassement de capacité ; renvoit sinon e.
extern int jcrd_add(jcrd *j, word *w, int file_index);

// jcrd_print_graph : tente d'écrire sur le flux pointé par stream un graphe
// d'appartenance de chaque mots à leur fichiers respectif. Les mots sont
// affiché dans l'ordre standard [...]. Renvoit un nombre >0 en cas d'échec
// d'écriture ; 0 en cas de succès.
extern int jcrd_print_graph(jcrd *j);

// jcrd_print_distance : Affiche la distance de Jaccard entre chaque paire de
// fichiers du tableau inputs_name, suivie des noms des fichiers correspondants.
//Si un nom de fichier est la chaîne vide "", il est affiché sous la forme ""
//pour représenter l'entrée standard.
extern void jcrd_print_distance(jcrd *j);

// jcrd_get_nb_files : Retourne le nombre de fichiers présents dans la structure
// jcrd.
extern int jcrd_get_nb_files(jcrd *j);

#endif
