CC=gcc
CFLAGS=-g -lm -Wall -Wextra -std=c99
DEPS=image_editor.c menu.h commands.h my_alloc.h my_defs.h read_print.h utils.h

OBJ:=commands.o
OBJ+=my_alloc.o
OBJ+=read_print.o
OBJ+=utils.o
OBJ+=menu.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

image_editor: $(DEPS) $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o image_editor