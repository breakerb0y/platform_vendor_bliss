## COPYRIGHT (C) 2005-2026 SICOM Systems Inc

lib_LTLIBRARIES = libsicomdetectposhw.la

libsicomdetectposhw_la_CFLAGS = $(CFLAG_VISIBILITY) $(PCI_CFLAGS) $(LIBUSB1_CFLAGS)
libsicomdetectposhw_la_SOURCES = \
	poshw.c minipc.c bigpc.c exotic.c virtual.c \
	util.c hwtable.c libdetectposhw.c
libsicomdetectposhw_la_LDFLAGS = $(CFLAG_VISIBILITY) -version-info $(LIBDETECTPOSHW_CURRENT):$(LIBDETECTPOSHW_REVISION):$(LIBDETECTPOSHW_AGE)
libsicomdetectposhw_la_LIBADD = $(PCI_LIBS) $(LIBUSB1_LIBS)

if NO_LA
install-exec-hook:
	$(RM) $(DESTDIR)$(libdir)/libsicomdetectposhw.la
endif
