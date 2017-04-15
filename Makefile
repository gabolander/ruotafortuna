#
# Semplice makefile per compilare i programmi necessari ..
#

# System environment
CC = gcc

CODEGEN =
OPTIMIZATION = -O0
# OPTIONS = -D_MG_BIG_ENDIAN_ -D_MG_NCURSES_
OPTIONS = 
DEBUG = -g
# DEBUG_FLAG = -DDEBUG
DEBUG_FLAG = 
PRJ=ruota.c
PRGS=ruota

# CFLAGS = $(CODEGEN) $(OPTIMIZATION) $(OPTIONS) $(DEBUG)
CFLAGS = $(CODEGEN) $(OPTIMIZATION) $(OPTIONS)

LIB = -lncurses
# LIB = 

COMPLINK = $(CC) $(CFLAGS) $(INCL) $(DEBUG) -Wall $(LIB)
COMPILE = $(CC) $(CFLAGS) $(INCL) $(DEBUG) $(DEBUG_FLAG) -Wall -c

SOURCES = ruota.c strfunz.c myutils.c
# OBJ = presta.o senal.o strfunz.o
OBJ = ${SOURCES:.c=.o}

# all: presta senal
all: ruota

# presta: strfunz.o presta.o
# 	$(COMPLINK) -o $@ presta.o strfunz.o
# senal: strfunz.o senal.o
# 	$(COMPLINK) -o $@ senal.o strfunz.o

# ruota: ruota.o myutils.o strfunz.o
# 	$(COMPLINK) -o $@ ruota.o
# 
ruota: ruota.o myutils.o strfunz.o
	$(COMPLINK) -o $@ $^

ruota.o: ruota.c ruota.h myproto.h
	$(COMPILE) ruota.c

strfunz.o: strfunz.c strfunz.h
	$(COMPILE) strfunz.c

myutils.o: myutils.c
	$(COMPILE) myutils.c

colori: colori.o
	$(COMPLINK) -o $@ colori.o
colori.o: colori.c
	$(COMPILE) colori.c

# strfunz.o: strfunz.c
# 	$(COMPILE) strfunz.c

clean:
	rm -f $(PRGS) $(OBJ) *~ colori colori.o
