CC = gcc
DEBUG = -g
CFLAGS = -Wall -Wextra -Wshadow -Wunreachable-code \
	-Wredundant-decls -Wmissing-declarations \
	-Wold-style-definition -Wmissing-prototypes \
	-Wdeclaration-after-statement -Wno-return-local-addr \
	-Wunsafe-loop-optimizations -Wuninitialized -Werror \
	-Wno-unused-parameter
PROG1 = whodat
PROGS = $(PROG1)

all: $(PROGS)

$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $@ $^

$(PROG1).o: $(PROG1).c $(INCLUDES)
	$(CC) $(CFLAGS) -c $<

clean cls:
	rm -f $(PROGS) *.o *~ \#*

tar:
	tar cvfa whodat_${LOGNAME}.tar.gz *.[ch] [mM]akefile *.bash


