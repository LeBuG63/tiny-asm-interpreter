PREFIX=.
SRCDIR=$(PREFIX)/src
INCDIR=$(PREFIX)/includes
BINDIR=$(PREFIX)/bin
OBJDIR=$(PREFIX)/obj

EXEC=$(BINDIR)/tinyasminterpreter
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -g
CPPFLAGS=-I$(INCDIR)
LDFLAGS=

vpath %.c src
vpath %.h include
vpath %.o obj

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/%.o: %.c
	- mkdir $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

.PHONY: clean mrproper

clean:
	rm $(OBJDIR)/*.o

mrproper: clean
	rm $(EXEC)