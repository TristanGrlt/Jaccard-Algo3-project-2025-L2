opt_dir = ../opt/
stack_dir = ../stack/
jaccard_dir = ../jaccard
avl_dir = ../avl
element_dir = ../element
word_dir = ../word
CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(opt_dir) -I$(stack_dir) -I$(jaccard_dir) -I$(avl_dir) -I$(element_dir) -I$(word_dir)\
  -DxMACRO
vpath %.c $(opt_dir) $(stack_dir) $(jaccard_dir) $(avl_dir) $(element_dir) $(word_dir)
vpath %.h $(opt_dir) $(stack_dir) $(jaccard_dir) $(avl_dir) $(element_dir) $(word_dir)
objects = main.o stack.o opt.o jaccard.o bst.o element.o word.o
executable = test
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c stack.h stack_ip.h opt.h jaccard.h
opt.o: opt.c opt.h stack.h stack_ip.h
stack.o: stack.c stack.h stack_ip.h
jaccard.o: jaccard.c jaccard.h bst.h bst_ip.h element.h
bst.o: bst.c bst.h bst_ip.h
element.o: element.c element.h word.h
word.o: word.c word.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
