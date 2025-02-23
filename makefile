cc = gcc
exe = suduku
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all : $(exe)

%.o : %.c
	$(cc) -o $@ -c $<

$(exe) : $(OBJ)
	$(cc) -o $@ $^

clean :
	rm -f $(OBJ)

.PHONY: all clean