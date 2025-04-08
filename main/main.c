
#include <stdio.h>
#include <stdlib.h>
#include "opt.h"

int main(int argc, char *argv[]) {
  opt *option = opt_empty();
  if (option == nullptr) {
    printf("error no more place");
    return EXIT_SUCCESS;
  }
  opt_create(option, argv, argc);
  //jcrd_init(option->)
  opt_dispose(&option);
  return EXIT_SUCCESS;
}
