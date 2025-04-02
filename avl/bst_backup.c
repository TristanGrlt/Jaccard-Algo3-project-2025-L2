//  bst.c : partie implantation d'un module polymorphe pour la spécification
//   ABINR du TDA ABinR(T).

#include "bst.h"

//=== Type cbst ================================================================

//--- Définition cbst ----------------------------------------------------------

typedef struct cbst cbst;

struct cbst {
  cbst *next[2];
  const void *ref;
};

//--- Raccourcis cbst ----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
#define REF(p)      ((p)->ref)
#define NEXT(p, d)  ((p)->next[(d) > 0])

//--- Divers -------------------------------------------------------------------

static size_t add__size_t(size_t x1, size_t x2) {
  return x1 + x2;
}

static size_t max__size_t(size_t x1, size_t x2) {
  return x1 > x2 ? x1 : x2;
}

static size_t min__size_t(size_t x1, size_t x2) {
  return x1 < x2 ? x1 : x2;
}

//--- Fonctions cbst -----------------------------------------------------------

//  DEFUN_CBST__MEASURE : définit la fonction récursive de nom « cbst__ ## fun »
//    et de paramètre un pointeur d'arbre binaire, qui renvoie zéro si l'arbre
//    est vide et « 1 + oper(r0, r1) » sinon, où r0 et r1 sont les valeurs
//    renvoyées par les appels récursifs de la fonction avec les pointeurs des
//    sous-arbres gauche et droit de l'arbre comme paramètres.
#define DEFUN_CBST__MEASURE(fun, oper)                                         \
  static size_t cbst__ ## fun(const cbst * p) {                                \
    return IS_EMPTY(p)                                                         \
      ? 0                                                                      \
      : 1 + oper(cbst__ ## fun(LEFT(p)), cbst__ ## fun(RIGHT(p)));             \
  }

//  cbst__size, cbst__height, cbst__distance : définitions.

DEFUN_CBST__MEASURE(size, add__size_t)
DEFUN_CBST__MEASURE(height, max__size_t)
DEFUN_CBST__MEASURE(distance, min__size_t)

//  cbst__dispose : libère les ressources allouées à l'arbre binaire pointé par
//    p.
static void cbst__dispose(cbst *p) {
  if (!IS_EMPTY(p)) {
    cbst__dispose(LEFT(p));
    cbst__dispose(RIGHT(p));
    free(p);
  }
}

static void *cbst__add_endofpath(cbst **pp, const void *ref, int (*compar)(
    const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    cbst *t = malloc(sizeof *t);
    if (t == nullptr) {
      return nullptr;
    }
    REF(t) = ref;
    RIGHT(t) = nullptr;
    LEFT(t) = nullptr;
    *pp = t;
    return (void *) ref;
  }
  int c = compar(REF(*pp), ref);
  if (c == 0) {
    return (void *) REF(*pp);
  }
  return cbst__add_endofpath(&NEXT(*pp, -c), ref, compar);
}

static void *cbst__remove_max(cbst **pp) {
  if (IS_EMPTY(RIGHT(*pp))) {
    cbst *t = *pp;
    *pp = LEFT(*pp);
    const void *ref = REF(t);
    free(t);
    return (void *) ref;
  }
  return cbst__remove_max(&RIGHT(*pp));
}

static void cbst__remove_root(cbst **pp) {
  if (IS_EMPTY(LEFT(*pp))) {
    cbst *t = *pp;
    *pp = RIGHT(*pp);
    free(t);
    return;
  }
  REF(*pp) = cbst__remove_max(&LEFT(*pp));
}

static void *cbst__remove_climbup_left(cbst **pp, const void *ref, int (*compar)(
    const void *,
    const void *)) {
  if (IS_EMPTY(*pp)) {
    return nullptr;
  }
  int c = compar(REF(*pp), ref);
  if (c == 0) {
    const void *t = REF(*pp);
    cbst__remove_root(pp);
    return (void *) t;
  }
  return cbst__remove_climbup_left(&NEXT(*pp, -c), ref, compar);
}

//  ICI, PROCHAINEMENT, LES SPÉCIFICATIONS ET DÉFINITIONS DE :
//    static size_t cbst__rank(const cbst *p, const void *ref,
//        int (*compar)(const void *, const void *), size_t rank);

static size_t cbst__rank(const cbst *p, const void *ref, int (*compar)(
    const void *, const void *), size_t rank) {
  if (IS_EMPTY(p)) {
    return rank;
  }
  int c = compar(p->ref, ref);
  if (c == 0) {
    return rank;
  }
  return cbst__rank(NEXT(p, -c), ref, compar,
      c
      > 0 ? rank - cbst__size(RIGHT(LEFT(p))) - 1 : rank
      + cbst__size(LEFT(RIGHT(p))) + 1);
}

static size_t cbst__number(const cbst *p, const void *ref, int (*compar)(
    const void *, const void *), size_t number) {
  if (IS_EMPTY(p)) {
    return number;
  }
  int c = compar(p->ref, ref);
  if (c == 0) {
    return number;
  }
  return cbst__number(NEXT(p, -c), ref, compar, (number * 2) + (c < 0));
}

static void *cbst__search(const cbst *p, const void *ref, int (*compar)(
    const void *, const void *)) {
  if (IS_EMPTY(p)) {
    return nullptr;
  }
  int c = compar(p->ref, ref);
  if (c == 0) {
    return (void *) p->ref;
  }
  return cbst__search(NEXT(p, -c), ref, compar);
}

#define REPR__TAB 4
#define REPR_SYM_GRAPHIC_EMPTY "|"

static void cbst__repr_graphic(const cbst *p, void (*put)(
    const void *ref), size_t level) {
  if (IS_EMPTY(p)) {
    printf("%*s%s\n", (int) level * REPR__TAB, "", REPR_SYM_GRAPHIC_EMPTY);
  } else {
    cbst__repr_graphic(RIGHT(p), put, level + 1);
    printf("%*s", (int) level * REPR__TAB, "");
    put(p->ref);
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

void *bst_remove_climbup_left(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__remove_climbup_left(&t->root, ref, t->compar);
}

void *bst_add_endofpath(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__add_endofpath(&t->root, ref, t->compar);
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

size_t bst_height(bst *t) {
  return cbst__height(t->root);
}

size_t bst_distance(bst *t) {
  return cbst__distance(t->root);
}

//  ICI, PROCHAINEMENT, LES DÉFINITIONS DE :
//    bst_rank

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
  return cbst__rank(t->root, ref, t->compar, cbst__size(LEFT(t->root)));
}

void bst_repr_graphic(bst *t, void (*put)(const void *ref)) {
  cbst__repr_graphic(t->root, put, 0);
}
