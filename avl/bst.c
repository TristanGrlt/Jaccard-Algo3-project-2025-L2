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
#define HEIGHT(p) ((p)->height)
#define NEXT(p, d)  ((p)->next[(d) > 0])
#define SIZE(p) ((p)->size)

//--- Divers -------------------------------------------------------------------

//static size_t add__size_t(size_t x1, size_t x2) {
//return x1 + x2;
//}

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

//DEFUN_CBST__MEASURE(size, add__size_t)
//DEFUN_CBST__MEASURE(height, max__size_t)
DEFUN_CBST__MEASURE(distance, min__int)

static size_t cbst__size(const cbst *p) {
  return IS_EMPTY(p) ? 0 : SIZE(p);
}

static void cbst__update_size(cbst *p) {
  SIZE(p) = cbst__size(LEFT(p)) + cbst__size(RIGHT(p)) + 1;
}

static int cbst__height(const cbst *p) {
  return p == nullptr ? 0 : p->height;
}

static void cbst__update_height(cbst *p) {
  HEIGHT(p) = max__int(cbst__height(LEFT(p)), cbst__height(RIGHT(p))) + 1;
}

static int cbst__balance(const cbst *p) {
  return p == nullptr ? 0 : cbst__height(LEFT(p)) - cbst__height(RIGHT(p));
}

static void cbst__rotation_right(cbst **pp) {
  cbst *p = LEFT(*pp);
  LEFT(*pp) = RIGHT(p);
  cbst__update_size(*pp);
  cbst__update_height(*pp);
  RIGHT(p) = *pp;
  *pp = p;
  cbst__update_size(*pp);
  cbst__update_height(*pp);
#ifdef BST_DEBUG
  printf("right\n");
#endif
}

static void cbst__rotation_left(cbst **pp) {
  cbst *p = RIGHT(*pp);
  RIGHT(*pp) = LEFT(p);
  cbst__update_size(*pp);
  cbst__update_height(*pp);
  LEFT(p) = *pp;
  *pp = p;
  cbst__update_size(*pp);
  cbst__update_height(*pp);
#ifdef BST_DEBUG
  printf("left\n");
#endif
}

static void cbst__rotation_left_right(cbst **pp) {
  cbst__rotation_left(&LEFT(*pp));
  cbst__rotation_right(pp);
}

static void cbst__rotation_right_left(cbst **pp) {
  cbst__rotation_right(&RIGHT(*pp));
  cbst__rotation_left(pp);
}

static int cbst__balancing(cbst **pp) {
  cbst__update_height(*pp);
  int b = cbst__balance(*pp);
  if (b == 2) {
    if (cbst__balance(LEFT(*pp)) == 1) {
      cbst__rotation_right(pp);
      return 21;
    }
    cbst__rotation_left_right(pp);
    return 22;
  }
  if (b == -2) {
    if (cbst__balance(RIGHT(*pp)) == -1) {
      cbst__rotation_left(pp);
      return -22;
    } else {
      cbst__rotation_right_left(pp);
      return -21;
    }
  }
  return 0;
}

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
    SIZE(t) = 1;
    HEIGHT(t) = 1;
    *pp = t;
    return (void *) ref;
  }
  int c = compar(ref, REF(*pp));
  if (c == 0) {
    return (void *) REF(*pp);
  }
  void *r = cbst__add_endofpath(&NEXT(*pp, c), ref, compar);
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return r;
}

static void *cbst__remove_max(cbst **pp) {
  if (IS_EMPTY(RIGHT(*pp))) {
    cbst *t = *pp;
    *pp = LEFT(*pp);
    const void *ref = REF(t);
    free(t);
    return (void *) ref;
  }
  void *r = cbst__remove_max(&RIGHT(*pp));
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return r;
}

static void cbst__remove_root(cbst **pp) {
  if (IS_EMPTY(LEFT(*pp))) {
    cbst *t = *pp;
    *pp = RIGHT(*pp);
    free(t);
    return;
  }
  REF(*pp) = cbst__remove_max(&LEFT(*pp));
  cbst__update_size(*pp);
  cbst__balancing(pp);
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
  void *r = cbst__remove_climbup_left(&NEXT(*pp, -c), ref, compar);
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return r;
}

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
  int c = compar(ref, REF(p));
  if (c == 0) {
    return number;
  }
  return cbst__number(NEXT(p, c), ref, compar, (number * 2) + (c > 0));
}

static void *cbst__search(const cbst *p, const void *ref, int (*compar)(
    const void *, const void *)) {
  if (IS_EMPTY(p)) {
    return nullptr;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return (void *) p->ref;
  }
  return cbst__search(NEXT(p, c), ref, compar);
}

#define REPR__TAB 4
#define REPR_SYM_GRAPHIC_EMPTY "|"

static void cbst__repr_graphic(const cbst *p, void (*put)(
    const void *ref), int level) {
  if (IS_EMPTY(p)) {
    printf("%*s%s\n", level * REPR__TAB, "", REPR_SYM_GRAPHIC_EMPTY);
  } else {
    cbst__repr_graphic(RIGHT(p), put, level + 1);
    printf("%*s", level * REPR__TAB, "");
    put(REF(p));
    printf("\n");
    cbst__repr_graphic(LEFT(p), put, level + 1);
  }
}

static int cbst_dft_infix_apply_context(cbst *p, int dir, void *context, int (*fun)(void *context, const void *ref), int (*fun_pre)(void *context), int (*fun_post)(void *context)){
  if (IS_EMPTY(p)) {
    return 0;
  }
  int r;
  if(fun_pre != nullptr) {
    if((r = fun_pre(context)) != 0) {
      return r;
    }
  }
  cbst_dft_infix_apply_context(NEXT(p, dir), dir, context, fun, fun_pre, fun_post);
  if((r = fun(context, REF(p))) != 0) {
    return r;
  }
  cbst_dft_infix_apply_context(NEXT(p, -dir), dir, context, fun, fun_pre, fun_post);
  if(fun_post != nullptr) {
    if((r = fun_post(context)) != 0) {
      return r;
    }
  }
  return 0;
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
  return (size_t) cbst__height(t->root);
}

size_t bst_distance(bst *t) {
  return (size_t) cbst__distance(t->root);
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
  return cbst__rank(t->root, ref, t->compar, cbst__size(LEFT(t->root)));
}

void bst_repr_graphic(bst *t, void (*put)(const void *ref)) {
  cbst__repr_graphic(t->root, put, 0);
}

int bst_dft_infix_apply_context(bst *t, int dir, void *context, int (*fun)(void *context, const void *ref), int (*fun_pre)(void *context), int (*fun_post)(void *context)){
  return cbst_dft_infix_apply_context(t->root, dir, context, fun, fun_pre, fun_post);
}




//
