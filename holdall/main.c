#include "holdall.h"
#include <stdio.h>
#include <string.h>

int pr(const char *ref) {
  printf("%s - ", ref);
  return 0;
}

int main(void) {
  char h[] = "d";
  char a[] = "aa";
  char b[] = "ab";
  char e[] = "cc";
  char c[] = "aa";
  char d[] = "ba";
  char f[] = "a";
  char g[] = "cc";
  holdall *ha = holdall_empty();
  holdall_put(ha, a);
  holdall_put(ha, b);
  holdall_put(ha, c);
  holdall_put(ha, d);
  holdall_put(ha, e);
  holdall_put(ha, f);
  holdall_put(ha, g);
  holdall_put(ha, h);
  holdall_apply(ha, (int (*)(void *))pr);
  holdall_sort(ha, (int (*)(const void *, const void *))strcmp);
  printf(" --> ");
  holdall_apply(ha, (int (*)(void *))pr);
  return 0;
}
