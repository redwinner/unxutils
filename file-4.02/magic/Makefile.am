pkgdata_DATA = magic magic.mime magic.mgc magic.mime.mgc

EXTRA_DIST = magic2mime Localstuff Header magic.mime $(magic_FRAGMENTS)

CLEANFILES =  magic magic.mgc magic.mime.mgc

magic: Header Localstuff $(magic_FRAGMENTS)
	cat $(srcdir)/Header $(srcdir)/Localstuff > $@
	for frag in $(magic_FRAGMENTS); do \
	  if test -f $(srcdir)/$$frag; then \
	    f=$(srcdir)/$$frag; \
	  else \
	    f=$$frag; \
	  fi; \
          cat $$f; \
	done >> $@

magic.mgc: magic
	$(top_builddir)/src/file -C -m $(srcdir)/magic

magic.mime.mgc: magic.mime
	$(top_builddir)/src/file -C -m $(srcdir)/magic.mime

magic_FRAGMENTS = \
Magdir/acorn \
Magdir/adi \
Magdir/adventure \
Magdir/allegro \
Magdir/alliant \
Magdir/alpha \
Magdir/amanda \
Magdir/amigaos \
Magdir/animation \
Magdir/apl \
Magdir/apple \
Magdir/applix \
Magdir/archive \
Magdir/asterix \
Magdir/att3b \
Magdir/audio \
Magdir/bFLT \
Magdir/blender \
Magdir/blit \
Magdir/bout \
Magdir/bsdi \
Magdir/c-lang \
Magdir/cddb \
Magdir/chi \
Magdir/chord \
Magdir/cisco \
Magdir/citrus \
Magdir/claris \
Magdir/clipper \
Magdir/commands \
Magdir/compress \
Magdir/console \
Magdir/convex \
Magdir/ctags \
Magdir/cvs \
Magdir/database \
Magdir/diamond \
Magdir/diff \
Magdir/digital \
Magdir/dolby \
Magdir/dump \
Magdir/dyadic \
Magdir/editors \
Magdir/elf \
Magdir/encore \
Magdir/epoc \
Magdir/filesystems \
Magdir/flash \
Magdir/fonts \
Magdir/frame \
Magdir/freebsd \
Magdir/fsav \
Magdir/gimp \
Magdir/gnu \
Magdir/grace \
Magdir/gringotts \
Magdir/hdf \
Magdir/hitachi-sh \
Magdir/hp \
Magdir/human68k \
Magdir/ibm370 \
Magdir/ibm6000 \
Magdir/iff \
Magdir/images \
Magdir/intel \
Magdir/interleaf \
Magdir/island \
Magdir/ispell \
Magdir/java \
Magdir/jpeg \
Magdir/karma \
Magdir/lecter \
Magdir/lex \
Magdir/lif \
Magdir/linux \
Magdir/lisp \
Magdir/mach \
Magdir/macintosh \
Magdir/magic \
Magdir/mail.news \
Magdir/maple \
Magdir/mathematica \
Magdir/mcrypt \
Magdir/mime \
Magdir/mips \
Magdir/mirage \
Magdir/mkid \
Magdir/mlssa \
Magdir/mmdf \
Magdir/modem \
Magdir/motorola \
Magdir/msdos \
Magdir/msvc \
Magdir/natinst \
Magdir/ncr \
Magdir/netbsd \
Magdir/netscape \
Magdir/news \
Magdir/nitpicker \
Magdir/octave \
Magdir/olf \
Magdir/os2 \
Magdir/os9 \
Magdir/osf1 \
Magdir/palm \
Magdir/parix \
Magdir/pbm \
Magdir/pdf \
Magdir/pdp \
Magdir/perl \
Magdir/pgp \
Magdir/pkgadd \
Magdir/plus5 \
Magdir/printer \
Magdir/project \
Magdir/psdbms \
Magdir/pulsar \
Magdir/pyramid \
Magdir/python \
Magdir/riff \
Magdir/rpm \
Magdir/rtf \
Magdir/sc \
Magdir/sccs \
Magdir/sendmail \
Magdir/sequent \
Magdir/sgml \
Magdir/sharc \
Magdir/sketch \
Magdir/smalltalk \
Magdir/sniffer \
Magdir/softquad \
Magdir/spectrum \
Magdir/sql \
Magdir/sun \
Magdir/sysex \
Magdir/teapot \
Magdir/terminfo \
Magdir/tex \
Magdir/tgif \
Magdir/ti-8x \
Magdir/timezone \
Magdir/troff \
Magdir/tuxedo \
Magdir/typeset \
Magdir/unknown \
Magdir/uuencode \
Magdir/varied.out \
Magdir/vax \
Magdir/vicar \
Magdir/visx \
Magdir/vms \
Magdir/vmware \
Magdir/vorbis \
Magdir/vxl \
Magdir/wordperfect \
Magdir/xdelta \
Magdir/xenix \
Magdir/zilog \
Magdir/zyxel
