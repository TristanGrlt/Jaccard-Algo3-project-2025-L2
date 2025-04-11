CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -DxWANT_HASHTABLE_EXT -DWANT_HOLDALL_EXT
objects = main.o holdall.o
executable = test
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c holdall.h holdall_ip.h
holdall.o: holdall.c holdall.h holdall_ip.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
