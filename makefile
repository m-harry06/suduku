cc = gcc
exe = sukudu
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all : $(exe)

%.o : %.c
	$(cc) -o $@ -c $<

$(exe) : $(OBJ)
	$(cc) -o $@ $^