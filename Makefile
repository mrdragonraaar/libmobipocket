CC = gcc
CCFLAGS = -Wall -ansi
INCDIR = include/
INCLUDES = -I$(INCDIR)
SRCDIR = src/
SRC = base_header.c common.c exth_header.c mobi_header.c mobipocket.c palmdoc_header.c pdb.c
OBJ = $(SRC:.c=.o)
LIB = libmobipocket.a
INSTALLDIR = /usr/local/
INSTALLDIRINC = $(INSTALLDIR)include/mobipocket/
INSTALLDIRLIB = $(INSTALLDIR)lib/

all: $(SRC) $(LIB)

$(SRC):
	$(CC) $(INCLUDES) $(CCFLAGS) -c $(SRCDIR)$@

$(LIB):
	ar rcs $(LIB) $(OBJ)
	ranlib $(LIB)

install:
	mkdir -p $(INSTALLDIRINC)
	cp $(INCDIR)*.h $(INSTALLDIRINC)
	cp $(LIB) $(INSTALLDIRLIB)

clean:
	rm -f $(OBJ) $(LIB)
