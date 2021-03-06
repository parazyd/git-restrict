.POSIX:

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# Use system flags
GR_CFLAGS = $(CFLAGS) -Wall -Wextra -Werror -pedantic -std=c99
GR_LDFLAGS = $(LDFLAGS) -static -s

BIN = git-restrict
MAN = $(BIN).1
OBJ = $(BIN:=.o)

all: $(BIN)

.c.o:
	$(CC) -c $(GR_CFLAGS) $<

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(GR_LDFLAGS) -o $@

clean:
	rm -f $(BIN) $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	cp -f $(MAN) $(DESTDIR)$(MANPREFIX)/man1
	chmod 755 $(DESTDIR)$(PREFIX)/bin/$(BIN)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(MANPREFIX)/man1/$(MAN)

test: all
	@./test.sh

.PHONY: all clean install uninstall test
