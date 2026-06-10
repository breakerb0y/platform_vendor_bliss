## COPYRIGHT (C) 2007-2026 SICOM Systems Inc (bdoan@sicompos.com)

include $(top_srcdir)/libsrc/Makefile.subtarget

AM_CPPFLAGS = $(SYSTEM_CFLAGS)

if HAVE_SYS_IO_H

if ENABLE_PCI
PCI_BINARIES = lxserial lxinfo
endif

SYS_IO_BINARIES = $(PCI_BINARIES) winbond sl20serial

endif

sbin_PROGRAMS = detectposhw $(SYS_IO_BINARIES)

lxserial_SOURCES = lxserial.c cs5536.c
lxserial_CFLAGS = $(PCI_CFLAGS)
lxserial_LDADD = $(PCI_LIBS)
if STATIC_EXECUTABLES
lxserial_LDFLAGS = -static-libtool-libs
endif

lxinfo_SOURCES = lxinfo.c cs5536.c
lxinfo_CFLAGS = $(PCI_CFLAGS)
lxinfo_LDADD = $(PCI_LIBS)
if STATIC_EXECUTABLES
lxinfo_LDFLAGS = -static-libtool-libs
endif

winbond_SOURCES = winbond.c

sl20serial_SOURCES = sl20serial.c

detectposhw_CFLAGS = \
	$(SYSTEM_CFLAGS) $(DETECTPOSHW_CFLAGS)

detectposhw_SOURCES = detectposhw.c
detectposhw_DEPENDENCIES = $(DETECTPOSHW_LIBS)
detectposhw_LDADD = $(DETECTPOSHW_LIBS)
if STATIC_EXECUTABLES
detectposhw_LDFLAGS = -static-libtool-libs
endif
