# location of header files
IDIR = ./include

# location of source files
SRCDIR = ./src

# compiler to user
CC = gcc

# compiler flags
CP = -Wall -pedantic
CFLAGS = -I$(IDIR)

# header dependencies
_DEPS = common.h card.h hand.h shoe.h
DEPS = $(patsubst %,$(IDIR)/$(_DEPS))

# object files
ODIR = $(SRCDIR)/obj
_OBJ = card.o hand.o shoe.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: blackjack

blackjack: $(SRCDIR)/main.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(CP)

$(ODIR)/%.o: $(SRCDIR)/%.c $(IDIR)/%.h
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
