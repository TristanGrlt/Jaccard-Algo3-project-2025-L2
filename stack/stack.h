//  stack.h : partie interface d'un module mutatif polymorphe pour la
//    spécification PILE du TDA Pile(T) ainsi que pour une extension
//    optionnelle.

//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

//  Fonctionnement général :
//  - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « stack * » ou
//      « stack ** » ont un comportement indéterminé lorsque ce paramètre ou sa
//      déréférence n'est pas l'adresse d'un contrôleur préalablement renvoyée
//      avec succès par la fonction stack_empty et non révoquée depuis par la
//      fonction stack_dispose ;
//  - aucune fonction ne peut ajouter un pointeur nul à la structure de
//      données ;
//  - les fonctions de type de retour « void * » renvoient un pointeur nul en
//      cas d'échec. En cas de succès, elles renvoient une référence
//      actuellement ou auparavant stockée par la structure de données ;
//  - l'implantation des fonctions dont la spécification ne précise pas qu'elles
//      doivent gérer les cas de dépassement de capacité ne doivent avoir
//      affaire à aucun problème de la sorte.

//  L'extension est formée des éventuelles déclarations et définitions qui
//    figurent aux lignes 111-147.

//  Les identificateurs introduits par l'extension ainsi que les identificateurs
//    de macro STACK_EXT et WANT_STACK_EXT sont réservés pour être utilisés
//    comme indiqué ci-après :
//  - lorsque le module peut mettre l'intégralité de l'extension à disposition
//      de l'utilisateurice, la macro STACK_EXT doit être définie dans l'en-tête
//      "stack_ip.h" ;
//  - dans le cas contraire, la macro STACK_EXT n'est pas définie dans l'en-tête
//      "stack_ip.h" et ne doit pas l'être au sein de l'unité de traduction de
//      prétraitement  ;
//  - la visibilité et l'implantation de l'extension ne sont effectives qu'à la
//      double condition que la macro STACK_EXT soit définie dans l'en-tête
//      "stack_ip.h" et que la macro WANT_STACK_EXT soit définie par
//      l'utilisateurice ;
//  - aucune modification du statut défini/non défini des macros STACK_EXT et
//      WANT_STACK_EXT ne doit intervenir au sein de l'unité de traduction de
//      prétraitement après que le présent en-tête ait été inclus pour la
//      première fois.

#ifndef STACK__H
#define STACK__H

//- STANDARD --v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v

#include <stdlib.h>

//  struct stack, stack : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer une pile de références sur des objets
//    quelconques.
typedef struct stack stack;

//  stack_empty : tente d'allouer les ressources nécessaires pour gérer une
//    nouvelle pile initialement vide. Renvoie un pointeur nul en cas de
//    dépassement de capacité. Renvoie sinon un pointeur vers le contrôleur
//    associé à la pile.
extern stack *stack_empty(void);

//  stack_dispose : sans effet si *sptr est un pointeur nul. Libère sinon les
//    ressources allouées à la gestion de la pile associée à *sptr puis affecte
//    un pointeur nul à *sptr.
extern void stack_dispose(stack **sptr);

//  stack_push : renvoie un pointeur nul si ref est un pointeur nul. Tente sinon
//    d'empiler ref sur la pile associée à s ; renvoie un pointeur nul en cas de
//    dépassement de capacité ; renvoie sinon ref.
extern void *stack_push(stack *s, const void *ref);

//  stack_pop : renvoie un pointeur nul si la pile associée à s est vide. Dépile
//    sinon la pile et renvoie la référence qui était au sommet de la pile avant
//    le dépilement.
extern void *stack_pop(stack *s);

//  stack_top : renvoie un pointeur nul si la pile associée à s est vide.
//    Renvoie sinon la référence qui est au sommet de la pile.
extern void *stack_top(stack *s);

//  stack_is_empty : renvoie true ou false selon que la pile associée à s est
//    vide ou non.
extern bool stack_is_empty(stack *s);

//  stack_height : renvoie la hauteur de la pile associée à s.
extern size_t stack_height(stack *s);

//- STANDARD --^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^

#undef STACK_EXT
#include "stack_ip.h"

#if defined STACK_EXT && defined WANT_STACK_EXT

//- EXTENSION -v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v---v

//  Sont ajoutées au standard quelques fonctions qui peuvent être utiles.
//  Certaines de ces fonctions ne peuvent être exprimées selon le standard sans
//    avoir affaire à un problème du dépassement de capacité qui n'est pas
//    terminal. Il est exigé de leurs implantations qu'elles évitent cet écueil.
//  Les autres pourraient être exprimées selon le standard sans avoir affaire
//    à un problème de dépassement de capacité ou, si elles ont affaire à lui,
//    de manière terminale uniquement. Il est attendu que leurs implantations se
//    montrent au moins aussi performantes que toute expression selon le
//    standard.

//  stack_clean : vide la pile associée à s. Renvoie s.
//  La fonction stack_clean pourrait être implantée selon le standard par :
//
//  stack *stack_clean(stack *s) {
//    while (stack_pop(s) != nullptr) {
//    }
//    return s;
//  }
//
extern stack *stack_clean(stack *s);

//  stack_reverse : renverse la pile associée à s. Renvoie s.
extern stack *stack_reverse(stack *s);

//  stack_bottom : renvoie un pointeur nul si la pile associée à s est vide.
//    Renvoie sinon la référence qui est au fond de la pile.
extern void *stack_bottom(stack *s);

//  stack_move : renvoie un pointeur nul si la pile associée à ssrc est vide ou
//    si ssrc et sdest sont associées à la même pile. Tente sinon d'empiler la
//    référence qui est au sommet de la pile associée à ssrc sur la pile
//    associée à sdest ; renvoie un pointeur nul en cas de dépassement de
//    capacité ; dépile sinon la pile associée à ssrc et renvoie la référence
//    qui était au sommet de la pile avant le dépilement.
//  La fonction stack_move pourrait être implantée selon le standard par :
//
//  void *stack_move(stack *sdest, stack *ssrc) {
//    if (stack_is_empty(ssrc) || sdest == ssrc) {
//      return nullptr;
//    }
//    if (stack_push(sdest, stack_top(ssrc)) == nullptr) {
//      return nullptr;
//    }
//    return stack_pop(ssrc);
//  }
//
extern void *stack_move(stack *sdest, stack *ssrc);

//- EXTENSION -^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^---^

#endif

#endif
