opt_dir = ../opt/
stack_dir = ../stack/
jaccard_dir = ../jaccard
element_dir = ../element
word_dir = ../word
hashtable_dir = ../hashtable
holdall_dir = ../holdall
CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(opt_dir) -I$(stack_dir) -I$(jaccard_dir) -I$(word_dir) -I$(element_dir) -I$(hashtable_dir -I$(holdall_dir)\
  -DxMACRO
vpath %.c $(opt_dir) $(stack_dir) $(jaccard_dir) $(element_dir) $(word_dir) $(hashtable_dir) $(holdall_dir)
vpath %.h $(opt_dir) $(stack_dir) $(jaccard_dir) $(element_dir) $(word_dir) $(hashtable_dir) $(holdall_dir)
objects = main.o stack.o opt.o jaccard.o element.o word.o hashtable.o holdall.o
executable = test
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c stack.h stack_ip.h opt.h jaccard.h holdall.h holdall_ip.h
opt.o: opt.c opt.h stack.h stack_ip.h
stack.o: stack.c stack.h stack_ip.h
jaccard.o: jaccard.c jaccard.h element.h
word.o: word.c word.h
element.o: element.c element.h word.h hashtable.h hashtable_ip.h
hashtable.o: hashtable.c hashtable.h hashtable_ip.h
holdall.o: holdall.c holdall.h holdall_ip.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
