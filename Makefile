SHELL = /bin/sh
CC = gcc
RELEASE = -O3
DEBUG = -g -Wall
CFLAGS = $(RELEASE)
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m644
bindir = /usr/bin
mandir = /usr/share/man
man1dir = $(mandir)/man1
prefix = /usr
datarootdir = $(prefix)/share

SOURCES = semt.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE = semt

all: $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o $(EXECUTABLE)

distclean: clean

maintainer-clean: clean

install:
	$(INSTALL_PROGRAM) -D $(EXECUTABLE) $(DESTDIR)$(bindir)/$(EXECUTABLE)
	$(INSTALL_DATA) -D ./man/semt.1 $(DESTDIR)$(man1dir)/semt.1
	$(INSTALL_DATA) -D ./edid.bin.example $(DESTDIR)$(datarootdir)/$(EXECUTABLE)/edid.bin.example

uninstall:
	rm $(man1dir)/semt.1 $(bindir)/$(EXECUTABLE) $(datarootdir)/$(EXECUTABLE)/edid.bin.example
