dnl 
dnl   Configure script for the SICOM Systems detectposhw module
dnl   COPYRIGHT (C) 2015-2026 SICOM Systems Inc (bdoan@sicompos.com)
dnl

dnl init autoconf
AC_INIT([detectposhw],[1.3.31],[http://www.sicompos.com])
AC_DEFINE(VERSION_TITLE,["RELEASE"],[Release Title])
AC_PREREQ([2.50])
AC_CONFIG_SRCDIR(config.h.in)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE()
AC_CONFIG_HEADERS(config.h)
AM_MAINTAINER_MODE

dnl check for progs/headers we want
AC_SEARCH_LIBS([strerror],[cposix])
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AM_PROG_CC_C_O
AM_PROG_AR
LT_INIT([shared disable-static])
LT_OUTPUT

gl_VISIBILITY
AS_IF([test x$gl_cv_cc_visibility = xyes],
	[AC_DEFINE(DLL_EXPORT,[__attribute__((visibility("default")))],[Explicitly export global symbols])],
	[AC_DEFINE(DLL_EXPORT,[],[Explicitly export global symbols])]
)

AC_TRY_LINK_FLAG([-Wl,--no-undefined],[ac_cv_Wl_no_undefined])
AC_TRY_LINK_FLAG([-Wl,--as-needed],[ac_cv_Wl_as_needed])
AC_TRY_LINK_FLAG([-Wl,--no-allow-shlib-undefined],[ac_cv_Wl_no_allow_shlib_undefined])

dnl
dnl Libtool's versioning system can be read at
dnl http://www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html
dnl The number triplet is CURRENT:REVISION:AGE, the library
dnl can provide several interface versions, represented by "CURRENT" and "AGE".
dnl The interface versions CURRENT-AGE, ..., CURRENT are supported by the library,
dnl with "REVISION" being the revision of the CURRENT interface version.
dnl The "CURRENT" and "AGE" are also encoded in the library version.
dnl Some examples:
dnl -version-info 1:0:0 -> libXXX.so.1.0.0, supports interface version 1
dnl -version-info 1:3:0 -> libXXX.so.1.0.3, supports interface version 1, rev 3
dnl -version-info 2:0:1 -> libXXX.so.1.1.0, supports interface version 1 & 2
dnl -version-info 2:3:1 -> libXXX.so.1.1.3, supports interface version 1 & 2, rev 3
dnl -version-info 2:3:0 -> libXXX.so.2.0.3, supports interface version 2, rev 3
dnl
LIBDETECTPOSHW_CURRENT=4
LIBDETECTPOSHW_REVISION=31
LIBDETECTPOSHW_AGE=3

AC_SUBST(LIBDETECTPOSHW_CURRENT)
AC_SUBST(LIBDETECTPOSHW_REVISION)
AC_SUBST(LIBDETECTPOSHW_AGE)

AS_IF([test x$enable_maintainer_mode = xyes],[WERRORFLAG=],[WERRORFLAG="-Werror"])

AC_CHECK_HEADERS(sys/io.h)
AM_CONDITIONAL(HAVE_SYS_IO_H, test x$ac_cv_header_sys_io_h = xyes)

AC_ARG_WITH([libtool-files],
	[AS_HELP_STRING([--without-libtool-files],
		[do not install libtool libname.la files])],
	[AS_IF([test x$with_libtool_files = xyes],[],[test x$with_libtool_files = xno],[],[AC_MSG_ERROR([--with-libtool-files must not need an argument])])],
	[with_libtool_files=yes])
AM_CONDITIONAL(NO_LA, test x$with_libtool_files = xno)

AC_ARG_ENABLE([static-executables],
	[AS_HELP_STRING([--enable-static-executables],
		[build executables statically linked to Sicom libraries])])
AM_CONDITIONAL(STATIC_EXECUTABLES, test x$enable_static_executables = xyes)

AC_ARG_WITH([pkgconfigdir],
	[AS_HELP_STRING([--with-pkgconfigdir=<dir>],
		[specify pkgconfig directory @<:@default=${libdir}/pkgconfig@:>@])],
	[if test x$with_pkgconfigdir = xyes; then
		AC_MSG_ERROR([--with-pkgconfigdir needs a directory PATH])
	 else
		pkgconfigdir=$with_pkgconfigdir
	 fi],
	[pkgconfigdir=${libdir}/pkgconfig])
AC_SUBST(pkgconfigdir)

AC_ARG_ENABLE([pci],
	[AS_HELP_STRING([--enable-pci],
		[whether to use libpci])],
	[], [enable_pci=yes])
AM_CONDITIONAL(ENABLE_PCI, test x$enable_pci = xyes)
AS_IF([test x$enable_pci = xyes],[
	AC_DEFINE(HAVE_PCI_ENABLED,[1],[libpci usage enable])
	PKG_CHECK_MODULES(PCI, libpci, [], [
		AC_CHECK_LIB([pci],[pci_alloc],[PCI_LIBS="-lpci"],[AC_MSG_ERROR([libpci is not found])])
	])])
AC_SUBST(PCI_CFLAGS)
AC_SUBST(PCI_LIBS)

PKG_CHECK_MODULES(LIBUSB1, libusb-1.0)
AC_SUBST(LIBUSB1_CFLAGS)
AC_SUBST(LIBUSB1_LIBS)

AC_SUBST(WERRORFLAG)
CFLAGS="-g -O2 -Wall ${WERRORFLAG} -Wchar-subscripts -D_GNU_SOURCE -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED -I\${top_srcdir}/include $CFLAGS"

DETECTPOSHW_CFLAGS="-I\${top_srcdir}/include"
DETECTPOSHW_LIBS="\$(top_srcdir)/libsrc/libsicomdetectposhw.la"
AC_SUBST(DETECTPOSHW_CFLAGS)
AC_SUBST(DETECTPOSHW_LIBS)

sicom_includedir="${includedir}/sicom"
AC_SUBST(sicom_includedir)

AC_CONFIG_FILES([
	Makefile
	include/Makefile
	libsrc/Makefile
	src/Makefile
	sicom-detectposhw.pc
])

AC_OUTPUT
