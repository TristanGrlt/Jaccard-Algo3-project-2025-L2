//  bst.c : partie implantation d'un module polymorphe pour la spécification
//   ABINR du TDA ABinR(T).

#include "bst.h"

//=== Type cbst ================================================================

//--- Définition cbst ----------------------------------------------------------

typedef struct cbst cbst;

struct cbst {
  size_t size;
  cbst *next[2];
  const void *ref;
  int height;
};

//--- Raccourcis cbst ----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
#define REF(p)      ((p)->ref)
#define HEIGHT(p)   ((p)->height)
#define NEXT(p, d)  ((p)->next[(d) > 0])
#define SIZE(p)     ((p)->size)
//--- Affichage de rotation ----------------------------------------------------
#define AVL_ROTATIONS
//--- Divers -------------------------------------------------------------------

static int max__int(int x1, int x2) {
  return x1 > x2 ? x1 : x2;
}

static int min__int(int x1, int x2) {
  return x1 < x2 ? x1 : x2;
}

//--- Fonctions cbst -----------------------------------------------------------

//  DEFUN_CBST__MEASURE : définit la fonction récursive de nom « cbst__ ## fun »
//    et de paramètre un pointeur d'arbre binaire, qui renvoie zéro si l'arbre
//    est vide et « 1 + oper(r0, r1) » sinon, où r0 et r1 sont les valeurs
//    renvoyées par les appels récursifs de la fonction avec les pointeurs des
//    sous-arbres gauche et droit de l'arbre comme paramètres.
#define DEFUN_CBST__MEASURE(fun, oper)                                         \
  static int cbst__ ## fun(const cbst * p) {                                \
    return IS_EMPTY(p)                                                         \
      ? 0                                                                      \
      : 1 + oper(cbst__ ## fun(LEFT(p)), cbst__ ## fun(RIGHT(p)));             \
  }

//  cbst__size, cbst__height, cbst__distance : définitions.
static size_t cbst__size(cbst *p) {
  return IS_EMPTY(p) ? 0 : SIZE(p);
}

static void cbst__update_size(cbst *p) {
  SIZE(p) = 1 + cbst__size(LEFT(p)) + cbst__size(RIGHT(p));
}

static int cbst__height(const cbst *p) {
  return IS_EMPTY(p) ? 0 : HEIGHT(p);
}

static void cbst__update_height(cbst *p) {
  HEIGHT(p) = 1 + max__int(cbst__height(LEFT(p)), cbst__height(RIGHT(p)));
}

static int cbst__balance(const cbst *p) {
  return IS_EMPTY(p) ? 0 : cbst__height(LEFT(p)) - cbst__height(RIGHT(p));
}

static void cbst__rotation_left(cbst **pp) {
#ifdef AVL_ROTATIONS
  printf("left\n");
#endif
  cbst *t = RIGHT(*pp);
  RIGHT(*pp) = LEFT(t);
  LEFT(t) = *pp;
  cbst__update_height(*pp);
  cbst__update_height(t);
  *pp = t;
}

static void cbst__rotation_right(cbst **pp) {
#ifdef AVL_ROTATIONS
  printf("right\n");
#endif
  cbst *t = LEFT(*pp);
  LEFT(*pp) = RIGHT(t);
  RIGHT(t) = *pp;
  cbst__update_height(*pp);
  cbst__update_height(t);
  *pp = t;
}

static void cbst__rotation_left_right(cbst **pp) {
#ifdef AVL_ROTATIONS
  printf("left-right\n");
#endif
  cbst__rotation_left(&LEFT(*pp));
  cbst__rotation_right(pp);
}

static void cbst__rotation_right_left(cbst **pp) {
#ifdef AVL_ROTATIONS
  printf("right-left\n");
#endif
  cbst__rotation_right(&RIGHT(*pp));
  cbst__rotation_left(&(*pp));
}

static int cbst__balancing(cbst **pp) {
  cbst__update_height(*pp);
  int b = cbst__balance(*pp);
  if (b > 1) {
    if (cbst__balance(LEFT(*pp)) < 0) {
      cbst__rotation_left_right(pp);
    } else {
      cbst__rotation_right(pp);
    }
    return -1;
  }
  if (b < -1) {
    if (cbst__balance(RIGHT(*pp)) > 0) {
      cbst__rotation_right_left(pp);
    } else {
      cbst__rotation_left(pp);
    }
    return -1;
  }
  return 0;
}

DEFUN_CBST__MEASURE(distance, min__int)

//  cbst__dispose : libère les ressources allouées à l'arbre binaire pointé par
//    p.
static void cbst__dispose(cbst *p) {
  if (!IS_EMPTY(p)) {
    cbst__dispose(LEFT(p));
    cbst__dispose(RIGHT(p));
    free(p);
  }
}

static void *cbst__add_endofpath(cbst **pp, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    *pp = malloc(sizeof **pp);
    if (*pp == nullptr) {
      return nullptr;
    }
    SIZE(*pp) = 1;
    HEIGHT(*pp) = 1;
    REF(*pp) = ref;
    LEFT(*pp) = EMPTY();
    RIGHT(*pp) = EMPTY();
    return (void *) ref;
  }
  int c = compar(ref, REF(*pp));
  if (c == 0) {
    return (void *) REF(*pp);
  }
  void *r = cbst__add_endofpath(&NEXT(*pp, c), ref, compar);
  if (r == ref) {
    cbst__update_size(*pp);
    cbst__balancing(pp);
  }
  return r;
}

//  cbst__remove_max : Recherche et supprime le plus grand élément de l'arbre
//    binaire de recherche pointé par *pp. Si l'élément à supprimer est la
//    racine du sous-arbre, il est remplacé par son sous-arbre gauche.
//    La fonction renvoie la référence de l'élément supprimé et met à jour
//    la taille de l'arbre.
static void *cbst__remove_max(cbst **pp) {
  if (IS_EMPTY(RIGHT(*pp))) {
    const void *ref = REF(*pp);
    cbst *t = *pp;
    *pp = LEFT(*pp);
    free(t);
    return (void *) ref;
  }
  void *result = cbst__remove_max(&RIGHT(*pp));
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return result;
}

//  cbst__remove_root : Supprime la racine de l'arbre binaire de recherche
//    pointé par *pp. Si la racine a un sous-arbre droit mais pas de sous-arbre
//    gauche, elle est remplacée par son sous-arbre droit. Sinon, elle est
//    remplacée par le plus grand élément de son sous-arbre gauche. La fonction
//    met à jour la taille de l'arbre après suppression
static void cbst__remove_root(cbst **pp) {
  if (IS_EMPTY(LEFT(*pp))) {
    cbst *t = *pp;
    *pp = RIGHT(*pp);
    free(t);
  } else {
    cbst *max_left = cbst__remove_max(&LEFT(*pp));
    REF(*pp) = max_left;
    cbst__update_size(*pp);
    //cbst__balancing(pp);
  }
  if (!IS_EMPTY(*pp)) {
    cbst__balancing(pp);
  }
}

static void *cbst__remove_climbup_left(cbst **pp, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    return nullptr;
  }
  int c = compar(ref, REF(*pp));
  if (c == 0) {
    const void *ref = REF(*pp);
    cbst__remove_root(pp);
    return (void *) ref;
  }
  void *r = cbst__remove_climbup_left(&NEXT(*pp, c), ref, compar);
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return r;
}

static void *cbst__search(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(p)) {
    return nullptr;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return (void *) REF(p);
  }
  return cbst__search(NEXT(p, c), ref, compar);
}

//ICI, PROCHAINEMENT, LES SPÉCIFICATIONS DE :
static size_t cbst__number(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *), size_t number) {
  if (IS_EMPTY(p)) {
    return number;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return number;
  }
  if (c < 0) {
    return cbst__number(LEFT(p), ref, compar, 2 * number);
  }
  return cbst__number(RIGHT(p), ref, compar, 2 * number + 1);
}

static size_t cbst__rank(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *), size_t rank) {
  if (IS_EMPTY(p)) {
    return rank;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return cbst__size(LEFT(p)) + rank;
  }
  return cbst__rank(NEXT(p,
        c), ref, compar,
        c < 0 ? rank : 1 + rank + cbst__size(LEFT(p)));
}

#define REPR__TAB 4

static void cbst__repr_graphic(const cbst *p,
    void (*put)(const void *ref), int level) {
  if (IS_EMPTY(p)) {
    return;
  } else {
    cbst__repr_graphic(RIGHT(p), put, level + 1);
    printf("%*s", level * REPR__TAB, "");
    put(REF(p));
    printf(" h : %d", cbst__height(p));
    //printf(" s : %zu", SIZE(p));
    printf(" b : %d", cbst__balance(p));
    printf("\n");
    cbst__repr_graphic(LEFT(p), put, level + 1);
  }
}

//=== Type bst =================================================================

//--- Définition bst -----------------------------------------------------------

struct bst {
  int (*compar)(const void *, const void *);
  cbst *root;
};

//--- Fonctions bst ------------------------------------------------------------

bst *bst_empty(int (*compar)(const void *, const void *)) {
  bst *t = malloc(sizeof *t);
  if (t == nullptr) {
    return nullptr;
  }
  t->compar = compar;
  t->root = EMPTY();
  return t;
}

void bst_dispose(bst **tptr) {
  if (*tptr == nullptr) {
    return;
  }
  cbst__dispose((*tptr)->root);
  free(*tptr);
  *tptr = nullptr;
}

void *bst_add_endofpath(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__add_endofpath(&t->root, ref, t->compar);
}

void *bst_remove_climbup_left(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__remove_climbup_left(&t->root, ref, t->compar);
}

void *bst_search(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__search(t->root, ref, t->compar);
}

size_t bst_size(bst *t) {
  return cbst__size(t->root);
}

int bst_height(bst *t) {
  return cbst__height(t->root);
}

int bst_distance(bst *t) {
  return cbst__distance(t->root);
}

size_t bst_number(bst *t, const void *ref) {
  if (ref == nullptr) {
    return (size_t) (-1);
  }
  return cbst__number(t->root, ref, t->compar, 1);
}

size_t bst_rank(bst *t, const void *ref) {
  if (ref == nullptr) {
    return (size_t) (-1);
  }
  return cbst__rank(t->root, ref, t->compar, 0);
}

void bst_repr_graphic(bst *t, void (*put)(const void *ref)) {
  cbst__repr_graphic(t->root, put, 0);
}
