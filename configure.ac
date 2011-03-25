#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([My GL], [2.0], [david@davecoss.com])
AC_CONFIG_SRCDIR([src/])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

AC_ARG_WITH([easybmp],AS_HELP_STRING([--with-easybmp%<:%=path%:>%],[Include EasyBMP library]),with_easybmp=${withval},)
if test x$with_easybmp != x; then
   AC_SUBST(EASYBMP_CPPFLAGS,"-I$with_mead/include/")
fi

AC_ARG_WITH([dnstd],AS_HELP_STRING([--with-dnstd],[Include libdnstd]),with_dnstd=${withval},AC_MSG_RESULT(Assuming libdnstd is installed in system paths))
if test x$with_dnstd != x; then
   AC_SUBST(DNSTD_CPPFLAGS,"-I $with_dnstd/include/")
fi



# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([alarm floor pow sqrt strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
