//  holdall.c : partie implantation du module holdall.

#include "holdall.h"

typedef struct choldall choldall;

struct choldall {
  void *ref;
  choldall *next;
};

struct holdall {
  choldall *head;
#if defined HOLDALL_PUT_TAIL
  choldall **tailptr;
#endif
  size_t count;
};

holdall *holdall_empty() {
  holdall *ha = malloc(sizeof *ha);
  if (ha == nullptr) {
    return nullptr;
  }
  ha->head = nullptr;
#if defined HOLDALL_PUT_TAIL
  ha->tailptr = &ha->head;
#endif
  ha->count = 0;
  return ha;
}

void holdall_dispose(holdall **haptr) {
  if (*haptr == nullptr) {
    return;
  }
  choldall *p = (*haptr)->head;
  while (p != nullptr) {
    choldall *t = p;
    p = p->next;
    free(t);
  }
  free(*haptr);
  *haptr = nullptr;
}

int holdall_put(holdall *ha, void *ref) {
  choldall *p = malloc(sizeof *p);
  if (p == nullptr) {
    return -1;
  }
  p->ref = ref;
#if defined HOLDALL_PUT_TAIL
  p->next = nullptr;
  *ha->tailptr = p;
  ha->tailptr = &p->next;
#else
  p->next = ha->head;
  ha->head = p;
#endif
  ha->count += 1;
  return 0;
}

size_t holdall_count(holdall *ha) {
  return ha->count;
}

int holdall_apply(holdall *ha,
    int (*fun)(void *)) {
  for (const choldall *p = ha->head; p != nullptr; p = p->next) {
    int r = fun(p->ref);
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ptr),
    int (*fun2)(void *ptr, void *resultfun1)) {
  for (const choldall *p = ha->head; p != nullptr; p = p->next) {
    int r = fun2(p->ref, fun1(context, p->ref));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ptr),
    void *context2, int (*fun2)(void *context2, void *ptr, void *resultfun1)) {
  for (const choldall *p = ha->head; p != nullptr; p = p->next) {
    int r = fun2(context2, p->ref, fun1(context1, p->ref));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

#if defined HOLDALL_EXT && defined WANT_HOLDALL_EXT
//  choldall_move_all_head : renvoie une valeur non nulle si *src et *dest
//    désignent le même objet (autrement dit si « src == dest ») ou si src est
//    vide. Déplace sinon en tête de la liste associée à *dest la suite des
//    cellules de la liste associée à *src et renvoie zéro (à la terminaison, la
//    liste associée à *src est vide et la liste associée à *dest est la
//    concaténation des listes originelles associées à *src et *dest).
static int choldall_move_all_head(choldall **src, choldall **dest) {
  if (*src == *dest) {
    return -1;
  }
  if (*src == nullptr) {
    return 1;
  }
  choldall **p = src;
  while (*p != nullptr) {
    p = &(*p)->next;
  }
  *p = *dest;
  *dest = *src;
  *src = nullptr;
  return 0;
}

//  choldall_move_head_head : renvoie une valeur non nulle si la liste associée
// à *src est vide. Déplace sinon la cellule de tête de la liste associée à *src
// vers la tête de la liste associée à *dest et renvoie zéro.
static int choldall_move_head_head(choldall **src, choldall **dest) {
  if (*src == nullptr) {
    return 1;
  }
  choldall *t = *dest;
  *dest = *src;
  *src = (*src)->next;
  (*dest)->next = t;
  return 0;
}

//  choldall_partition_pivot : sans effet si la liste associée à c est vide.
//    Déplace sinon respectivement dans les listes associées à *clth, *ceq et
//    *cgth, les cellules de la liste associée à c dont les références sont
//    strictement inférieures, égales et strictement supérieures au sens de la
//    fonction de comparaison compar à la référence de la cellule originellement
//    en tête de la liste associée à c (à la terminaison, la liste associée à c
//    est vide).
static void choldall_partition_pivot(choldall **c, choldall **clth,
    choldall **ceq, choldall **cgth, int (*compar)(
    const void *,
    const void *)) {
  const void *pivot = (*c)->ref;
  choldall_move_head_head(c, ceq);
  while ((*c) != nullptr) {
    int b = compar(pivot, (*c)->ref);
    if (b == 0) {
      choldall_move_head_head(c, ceq);
    } else if (b < 0) {
      choldall_move_head_head(c, cgth);
    } else {
      choldall_move_head_head(c, clth);
    }
  }
}

//  choldall_qsort : tente de recomposer la liste dynamique simplement chainé
// associée à c selon la fonction compar appliquée aux références qui s'y
// trouve. en utilisant la méthode du tri rapide.
static void choldall_qsort(choldall **c, int (*compar)(const void *,
    const void *)) {
  if (*c == nullptr) {
    return;
  }
  choldall *clth = nullptr;
  choldall *ceq = nullptr;
  choldall *cgth = nullptr;
  choldall_partition_pivot(c, &clth, &ceq, &cgth, compar);
  choldall_qsort(&clth, compar);
  choldall_qsort(&cgth, compar);
  choldall_move_all_head(&cgth, c);
  choldall_move_all_head(&ceq, c);
  choldall_move_all_head(&clth, c);
  return;
}

void holdall_sort(holdall *ha, int (*compar)(const void *, const void *)) {
  if (ha != nullptr && ha->head != nullptr) {
    choldall_qsort(&ha->head, compar);
  }
}

#endif
