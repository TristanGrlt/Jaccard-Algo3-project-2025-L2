//  stack.c : partie implantation d'un module mutatif polymorphe pour la
//    spécification PILE du TDA Pile(T).

#include "stack.h"

//  struct stack, stack : gestion des adresses des objets dans la pile à l'aide
//    d'une liste dynamique simplement chainée, l'ordre dans la liste étant
//    celui inverse à l'arrivée dans la pile. Le composant head a pour valeur le
//    pointeur de tête de la liste, le composant height mémorise la longueur de
//    la liste. Le composant head repère donc la cellule associée au sommet de
//    la pile lorsqu'elle n'est pas vide, et le composant height a donc pour
//    valeur la hauteur de la pile.

typedef struct cstack cstack;

struct cstack {
  const void *ref;
  cstack *next;
};

struct stack {
  cstack *head;
  size_t height;
};

stack *stack_empty(void) {
  stack *s = malloc(sizeof *s);
  if (s == nullptr) {
    return nullptr;
  }
  s->head = nullptr;
  s->height = 0;
  return s;
}

void stack_dispose(stack **sptr) {
  if (*sptr == nullptr) {
    return;
  }
  cstack *p = (*sptr)->head;
  while (p != nullptr) {
    cstack *t = p;
    p = p->next;
    free(t);
  }
  free(*sptr);
  *sptr = nullptr;
}

void *stack_push(stack *s, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  cstack *p = malloc(sizeof *p);
  if (p == nullptr) {
    return nullptr;
  }
  p->ref = ref;
  p->next = s->head;
  s->head = p;
  s->height += 1;
  return (void *) ref;
}

void *stack_pop(stack *s) {
  if (s->height == 0) {
    return nullptr;
  }
  cstack *p = s->head;
  s->head = p->next;
  const void *r = p->ref;
  free(p);
  s->height -= 1;
  return (void *) r;
}

void *stack_top(stack *s) {
  if (s->height == 0) {
    return nullptr;
  }
  return (void *) s->head->ref;
}

bool stack_is_empty(stack *s) {
  return s->height == 0;
}

size_t stack_height(stack *s) {
  return s->height;
}

#if defined STACK_EXT && defined WANT_STACK_EXT

stack *stack_clean(stack *s) {
  while (stack_pop(s) != nullptr) {
  }
  return s;
}

stack *stack_reverse(stack *s){
  void * ref;
  stack *s2 = stack_empty();
  while ((ref = stack_pop(s)) != nullptr) {
    stack_push(s2, ref);
  }
  stack_dispose(&s);
  return s2;
}

void *stack_move(stack *sdest, stack *ssrc) {
  if (stack_is_empty(ssrc) || sdest == ssrc) {
    return nullptr;
  }
  if (stack_push(sdest, stack_top(ssrc)) == nullptr) {
    return nullptr;
  }
  return stack_pop(ssrc);
}

void *stack_bottom(stack *s){
  if (stack_is_empty(s)){
    return nullptr;
  }
  s = stack_reverse(s);
  const void * ref = stack_top(s);
  s = stack_reverse(s);
  return (void *) ref;
}

#endif












