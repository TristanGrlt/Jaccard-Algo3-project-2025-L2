CC = gcc
CFLAGS = -std=c2x -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2 -I. -Istack

test: test.o opt.o stack/stack.o
	$(CC) $(CFLAGS) -o test test.o opt.o stack/stack.o

test.o: opt/test.c opt/opt.h
	$(CC) $(CFLAGS) -c opt/test.c

opt.o: opt/opt.c opt/opt.h stack/stack.h
	$(CC) $(CFLAGS) -c opt/opt.c

stack/stack.o: stack/stack.c stack/stack.h stack/stack_ip.h
	$(CC) $(CFLAGS) -c stack/stack.c

clean:
	rm -f test test.o opt.o stack/stack.o