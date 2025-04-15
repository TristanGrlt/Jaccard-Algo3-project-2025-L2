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

static void choldall_move_head_tail(choldall **src, choldall **destt) {
  if (*src == nullptr) {
    return;
  }
  choldall *n = *src;
  *src = (*src)->next;
  n->next = nullptr;
  *destt = n;
}

static void choldall_split_half(choldall **c, choldall **left, choldall **right, size_t size) {
  *left = nullptr;
  *right = nullptr;
  size_t i = 0;
  size_t mid = size / 2;
  while (*c != nullptr) {
    if (i < mid) {
      choldall_move_head_head(c, left);
      i++;
    } else {
      choldall_move_head_head(c, right);
    }
  }
}

static void choldall_merge(choldall **c, choldall *left, choldall *right, int (*compar)(const void *, const void *)) {
  choldall *r = nullptr;
  choldall **t = &r;
  while (left != nullptr || right != nullptr) {
    if (right == nullptr || (left != nullptr && compar(left->ref, right->ref) <= 0)) {
      choldall_move_head_tail(&left, t);
    } else {
      choldall_move_head_tail(&right, t);
    }
    t = &((*t)->next);
  }
  *c = r;
}

static void choldall_merge_sort(choldall **c, size_t size, int (*compar)(const void *, const void *)) {
  if (size <= 1) {
    return;
  }
  choldall *left = nullptr;
  choldall *right = nullptr;
  choldall_split_half(c, &left, &right, size);
  size_t m = size / 2;
  choldall_merge_sort(&left, m, compar);
  choldall_merge_sort(&right, size - m, compar);
  choldall_merge(c, left, right, compar);
}

void holdall_sort(holdall *ha, int (*compar)(const void *, const void *)) {
  if (ha != nullptr && ha->head != nullptr && ha->count > 1) {
    choldall_merge_sort(&ha->head, ha->count, compar);
#if defined HOLDALL_PUT_TAIL
    choldall *t = ha->head;
    while (t->next != nullptr) {
      t = t->next;
    }
    ha->tailptr = &(t->next);
#endif
  }
}

#endif
