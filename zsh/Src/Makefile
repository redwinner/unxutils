# Generated automatically from Makefile.in by configure.
#
# $Id: Makefile.in,v 2.3 1996/06/28 02:04:22 hzoli Exp $
#
# Makefile for Src subdirectory
#
# Copyright (c) 1996 Richard Coleman
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in
# all copies of this software.
#
# In no event shall Richard Coleman or the Zsh Development Group be liable
# to any party for direct, indirect, special, incidental, or consequential
# damages arising out of the use of this software and its documentation,
# even if Richard Coleman and the Zsh Development Group have been advised of
# the possibility of such damage.
#
# Richard Coleman and the Zsh Development Group specifically disclaim any
# warranties, including, but not limited to, the implied warranties of
# merchantability and fitness for a particular purpose.  The software
# provided hereunder is on an "as is" basis, and Richard Coleman and the
# Zsh Development Group have no obligation to provide maintenance,
# support, updates, enhancements, or modifications.
#

# zsh version
VERSION = 3.0.5

#SHELL = /bin/sh

top_srcdir = ..
srcdir     = .

prefix      = /usr/local
exec_prefix = $(prefix)
bindir      = $(exec_prefix)/bin

CC       = cl
CPPFLAGS = 
DEFS     = -DHAVE_CONFIG_H -DHAVE_DIRENT_H -DWINNT
## optimize for SIZE, not speed since there are no big algorithms anyway
CFLAGS   =  -YX -J -nologo -O1
#CFLAGS   =  -W3 -WX -YX -J -nologo -O2
#CFLAGS   =  -Wall -Wno-implicit -Wmissing-prototypes -O2
LDFLAGS  =  -nologo -pdb:none -entry:silly_entry
LINK     = link
LIBS     =  -nodefaultlib kernel32.lib user32.lib
#d:\mstools\lib\imagehlp.lib #-ltermcap

!if "$(DBG)" == ""
!error "usage: nmake DBG=(0|1) [DLL=1]"
!else
!if "$(DBG)" == "1"
DEFS = $(DEFS) -DNTDBG -DDBG -DDEBUG -DZSH_HASH_DEBUG
#DEFS = $(DEFS) -DZSH_MEM_DEBUG -DZSH_MEM_WARNING
CFLAGS = $(CFLAGS) -Zi -Od -W3 -WX
LDFLAGS = $(LDFLAGS) -debug:full
!endif
!endif

!if "$(DLL)" != ""
CFLAGS = -MD $(CFLAGS)
LDFLAGS = $(LDFLAGS) msvcrt.lib oldnames.lib advapi32.lib
!else
CFLAGS = -MT $(CFLAGS)
LDFLAGS = $(LDFLAGS) libcmt.lib oldnames.lib advapi32.lib
!endif

INCLUDES = -I.. -I. -I$(srcdir) -Intport -Id:\mstools\include

NTRES =BSDLOGO
NT_SRC = ntport\support.c ntport\bogus.c ntport\dirent.c \
	 ntport\io.c ntport\fork.c ntport\globals.c ntport\ntb1.c \
	 ntport\ntb2.c ntport\signal.c ntport\termcap.c ntport\tparam.c \
	 ntport\console.c ntport\tparse.c ntport\stdio.c ntport\zsh_nt.c\
	 ntport\ntbin.c ntport\ps.c
#	 ntport\except.c

NT_OBJ = support.o bogus.o dirent.o io.o fork.o globals.o signal.o \
	termcap.o tparam.o console.o tparse.o stdio.o zsh_nt.o ntbin.o ps.o
#except.o
NT_BOOKEND1 = ntb1.o
NT_BOOKEND2 = ntb2.o
NT_HDRS = ntport\ntport.h ntport\signal.h

COMPILE = $(CC) -c $(INCLUDES) $(CPPFLAGS) $(DEFS) $(CFLAGS)
LINK    = $(LINK) $(LDFLAGS) -out:$@

#INSTALL         = /usr/bin/ginstall -c
#INSTALL_PROGRAM = $(INSTALL)

AWK = awk
SED = sed --text

RM = -del

.SUFFIXES:
.SUFFIXES: .c .o .pro

.c.o:
	$(COMPILE) -Fo$*.o $<

.c.pro:
	$(SED) -n -f $(srcdir)/makepro.sed $< > $@

# this is for ansi2krn conversion
U = 

# this header file is parsed to generate signal names (signames.h)
SIGNAL_H = ntport/signal.h #/usr/include/asm/signal.h

# headers included in distribution
DIST_HDRS = version.h globals.h hashtable.h prototypes.h signals.h \
system.h zle.h zsh.h ztype.h

# generated headers
GEN_HDRS = signames.h ../config.h

# zsh headers necessary for compilation
HDRS = $(DIST_HDRS) $(GEN_HDRS) $(NT_HDRS)

# zsh C source
SRCS = builtin.c compat.c cond.c exec.c glob.c hashtable.c hist.c init.c \
input.c jobs.c lex.c linklist.c loop.c math.c mem.c params.c parse.c \
signals.c subst.c text.c utils.c watch.c zle_bindings.c zle_hist.c \
zle_main.c zle_misc.c zle_move.c zle_refresh.c zle_tricky.c zle_utils.c \
zle_vi.c zle_word.c $(NT_SRC)

# generated prototypes
PROTO = builtin.pro compat.pro cond.pro exec.pro glob.pro hashtable.pro \
hist.pro init.pro input.pro jobs.pro lex.pro linklist.pro loop.pro \
math.pro mem.pro params.pro parse.pro signals.pro subst.pro text.pro \
utils.pro watch.pro zle_bindings.pro zle_hist.pro zle_main.pro \
zle_misc.pro zle_move.pro zle_refresh.pro zle_tricky.pro zle_utils.pro \
zle_vi.pro zle_word.pro

# object files
OBJS = $Ubuiltin.o $Ucompat.o $Ucond.o $Uexec.o $Uglob.o $Uhashtable.o \
$Uhist.o $Uinit.o $Uinput.o $Ujobs.o $Ulex.o $Ulinklist.o $Uloop.o \
$Umath.o $Umem.o $Uparams.o $Uparse.o $Usignals.o $Usubst.o $Utext.o \
$Uutils.o $Uwatch.o $Uzle_bindings.o $Uzle_hist.o $Uzle_main.o \
$Uzle_misc.o $Uzle_move.o $Uzle_refresh.o $Uzle_tricky.o $Uzle_utils.o \
$Uzle_vi.o $Uzle_word.o $(NT_OBJ)

# auxiliary files
AUX = Makefile.in .indent.pro signames.awk makepro.sed ansi2knr.c TAGS tags

# all files in this directory included in the distribution
DIST = $(DIST_HDRS) $(SRCS) $(AUX)

# ========= DEPENDENCIES FOR BUILDING ==========

# default target
all: zsh.exe

zsh.exe: $(PROTO) $(OBJS) $(NT_BOOKEND1) $(NT_BOOKEND2) ntport/$(NTRES).res
	$(LINK) $(NT_BOOKEND1) $(OBJS) $(NT_BOOKEND2) \
	$(LIBS) ntport/$(NTRES).res \
	-section:.fusrdata,rw -map:zsh.map
#	-section:.fork_data,rw 

symbols:zsh.exe
	@echo finding image base
	@echo rebase may crash if msdev is holding the symbol file
	@dumpbin /headers zsh.exe | findstr /c:"image base"
	@echo The address given to rebase should match the image base
	rebase -b 400000 -x ..\symbols -a zsh.exe

chlog:
	tcsh -fc "echo \<XMP\> > ntport\\Changelog.html"
	type ntport\Changelog >> ntport\ChangeLog.html
	tcsh -fc "echo \</XMP\> >> ntport\\ChangeLog.html"

ansi2knr: ansi2knr.o
	$(LINK) ansi2knr.o

signames.h: signames.awk
	$(AWK) -f $(srcdir)/signames.awk $(SIGNAL_H) > signames.h

$(OBJS): $(HDRS)

$(PROTO): makepro.sed

# ========= DEPENDENCIES FOR NT ==========
support.o: ntport\support.c
	$(COMPILE) -Fo$*.o ntport\$*.c
bogus.o: ntport\bogus.c
	$(COMPILE) -Fo$*.o ntport\$*.c
dirent.o: ntport\dirent.c
	$(COMPILE) -Fo$*.o ntport\$*.c
io.o: ntport\io.c
	$(COMPILE) -Fo$*.o ntport\$*.c
fork.o: ntport\fork.c
	$(COMPILE) -Fo$*.o ntport\$*.c
globals.o: ntport\globals.c
	$(COMPILE) -Fo$*.o ntport\$*.c
signal.o: ntport\signal.c
	$(COMPILE) -Fo$*.o ntport\$*.c
termcap.o: ntport\termcap.c
	$(COMPILE) -Fo$*.o ntport\$*.c
tparam.o: ntport\tparam.c
	$(COMPILE) -Fo$*.o ntport\$*.c
tparse.o: ntport\tparse.c
	$(COMPILE) -Fo$*.o ntport\$*.c
console.o: ntport\console.c
	$(COMPILE) -Fo$*.o ntport\$*.c
stdio.o: ntport\stdio.c
	$(COMPILE) -Fo$*.o ntport\$*.c
zsh_nt.o: ntport\zsh_nt.c
	$(COMPILE) -Fo$*.o ntport\$*.c
ntbin.o: ntport\ntbin.c
	$(COMPILE) -Fo$*.o ntport\$*.c
ps.o: ntport\ps.c
	$(COMPILE) -Fo$*.o ntport\$*.c
ntb1.o: ntport\ntb1.c
	$(COMPILE) -Fo$*.o ntport\$*.c
ntb2.o: ntport\ntb2.c
	$(COMPILE) -Fo$*.o ntport\$*.c
ntport/$(NTRES).res: ntport/$(NTRES).rc
	rc -v -r ntport/$(NTRES).rc
#except.o: ntport\except.c
#	$(COMPILE) -Fo$*.o ntport\$*.c
# ========== DEPENDENCIES FOR INSTALLING ==========

install: install.bin

uninstall: uninstall.bin

# install binary, creating install directory if necessary
install.bin: zsh
	$(top_srcdir)/mkinstalldirs $(bindir)
	-if [ -f $(bindir)/zsh ]; then mv $(bindir)/zsh $(bindir)/zsh.old; fi
	$(INSTALL_PROGRAM) zsh $(bindir)/zsh
	-if [ -f $(bindir)/zsh-$(VERSION) ]; then rm -f $(bindir)/zsh-$(VERSION); fi
	ln $(bindir)/zsh $(bindir)/zsh-$(VERSION)

# uninstall binary
uninstall.bin:
	-if [ -f $(bindir)/zsh ]; then rm -f $(bindir)/zsh; fi
	-if [ -f $(bindir)/zsh-$(VERSION) ]; then rm -f $(bindir)/zsh-$(VERSION); fi

# ========== DEPENDENCIES FOR ANSI TO KNR CONVERSION ==========

_builtin.c: builtin.c ansi2knr
	./ansi2knr $(srcdir)/builtin.c > _builtin.c
_compat.c: compat.c ansi2knr
	./ansi2knr $(srcdir)/compat.c > _compat.c
_cond.c: cond.c ansi2knr
	./ansi2knr $(srcdir)/cond.c > _cond.c
_exec.c: exec.c ansi2knr
	./ansi2knr $(srcdir)/exec.c > _exec.c
_glob.c: glob.c ansi2knr
	./ansi2knr $(srcdir)/glob.c > _glob.c
_hashtable.c: hashtable.c ansi2knr
	./ansi2knr $(srcdir)/hashtable.c > _hashtable.c
_hist.c: hist.c ansi2knr
	./ansi2knr $(srcdir)/hist.c > _hist.c
_init.c: init.c ansi2knr
	./ansi2knr $(srcdir)/init.c > _init.c
_input.c: input.c ansi2knr
	./ansi2knr $(srcdir)/input.c > _input.c
_jobs.c: jobs.c ansi2knr
	./ansi2knr $(srcdir)/jobs.c > _jobs.c
_lex.c: lex.c ansi2knr
	./ansi2knr $(srcdir)/lex.c > _lex.c
_linklist.c: linklist.c ansi2knr
	./ansi2knr $(srcdir)/linklist.c > _linklist.c
_loop.c: loop.c ansi2knr
	./ansi2knr $(srcdir)/loop.c > _loop.c
_math.c: math.c ansi2knr
	./ansi2knr $(srcdir)/math.c > _math.c
_mem.c: mem.c ansi2knr
	./ansi2knr $(srcdir)/mem.c > _mem.c
_params.c: params.c ansi2knr
	./ansi2knr $(srcdir)/params.c > _params.c
_parse.c: parse.c ansi2knr
	./ansi2knr $(srcdir)/parse.c > _parse.c
_signals.c: signals.c ansi2knr
	./ansi2knr $(srcdir)/signals.c > _signals.c
_subst.c: subst.c ansi2knr
	./ansi2knr $(srcdir)/subst.c > _subst.c
_text.c: text.c ansi2knr
	./ansi2knr $(srcdir)/text.c > _text.c
_utils.c: utils.c ansi2knr
	./ansi2knr $(srcdir)/utils.c > _utils.c
_watch.c: watch.c ansi2knr
	./ansi2knr $(srcdir)/watch.c > _watch.c
_zle_bindings.c: zle_bindings.c ansi2knr
	./ansi2knr $(srcdir)/zle_bindings.c > _zle_bindings.c
_zle_hist.c: zle_hist.c ansi2knr
	./ansi2knr $(srcdir)/zle_hist.c > _zle_hist.c
_zle_main.c: zle_main.c ansi2knr
	./ansi2knr $(srcdir)/zle_main.c > _zle_main.c
_zle_misc.c: zle_misc.c ansi2knr
	./ansi2knr $(srcdir)/zle_misc.c > _zle_misc.c
_zle_move.c: zle_move.c ansi2knr
	./ansi2knr $(srcdir)/zle_move.c > _zle_move.c
_zle_refresh.c: zle_refresh.c ansi2knr
	./ansi2knr $(srcdir)/zle_refresh.c > _zle_refresh.c
_zle_tricky.c: zle_tricky.c ansi2knr
	./ansi2knr $(srcdir)/zle_tricky.c > _zle_tricky.c
_zle_utils.c: zle_utils.c ansi2knr
	./ansi2knr $(srcdir)/zle_utils.c > _zle_utils.c
_zle_vi.c: zle_vi.c ansi2knr
	./ansi2knr $(srcdir)/zle_vi.c > _zle_vi.c
_zle_word.c: zle_word.c ansi2knr
	./ansi2knr $(srcdir)/zle_word.c > _zle_word.c

# ========== DEPENDENCIES FOR CLEANUP ==========

mostlyclean:
	$(RM)  *.o  *.pdb *.pch

clean: mostlyclean
	$(RM)  signames.h _*.c *.pro
	cd ntport
	$(RM) *.res
	cd ..
cleanall: clean
	$(RM) *.mdp *.map *.exe *.dbg

distclean: clean
	$(RM) -f Makefile

realclean: distclean
	$(RM) -f TAGS tags

superclean: realclean

# ========== DEPENDENCIES FOR MAINTENANCE ==========

subdir = Src

Makefile: Makefile.in ../config.status
	cd .. && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= ./config.status

# tag file for vi
tags: TAGS
	cd $(srcdir) && ctags -w $(SRCS) $(DIST_HDRS) && \
	if test -d RCS; then \
	    rm -f RCS/tags,v RCS/TAGS,v && \
	    ci -t-tags -m'#tags' tags TAGS && \
	    rcs -U tags TAGS && \
	    co TAGS tags ; \
	fi

# tag file for emacs
TAGS: $(SRCS) $(DIST_HDRS)
	cd $(srcdir) && etags $(SRCS) $(DIST_HDRS)

distdir = ../`cat ../distname`/$(subdir)
dist: $(DIST)
	@echo "Copying distribution files in $(subdir)"
	for file in $(DIST); do    \
	  cp -p $$file $(distdir); \
	done

rcsdistdir = ../`cat ../rcsdistname`/$(subdir)
dist-rcs: $(DIST)
	@echo "Copying distribution files in $(subdir) (RCS source)"
	for file in $(DIST); do                 \
	  ci $$file;                            \
	  cp -p RCS/$$file,v $(rcsdistdir)/RCS; \
	  co -l $$file;                         \
	done
