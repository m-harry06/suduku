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
	del /S /Q *.o
.PHONY: all clean