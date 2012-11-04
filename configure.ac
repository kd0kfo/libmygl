#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([My GL], [2.2.1], [david@davecoss.com])
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
   AC_SUBST(DNSTD_LDFLAGS,"-L$with_dnstd/lib")
   AC_SUBST(DNSTD_LDADD,"-ldnstd")
fi

AC_ARG_WITH([netcdf],AS_HELP_STRING([--with-netcdf],[Include netcdf]),with_netcdf=${withval},AC_MSG_RESULT(Assuming NetCDF is installed in system paths))
if test x$with_dnstd != x; then
   AC_SUBST(NETCDF_CPPFLAGS,"-I $with_netcdf/include/")
   AC_SUBST(NETCDF_LDFLAGS,"-L$with_netcdf/lib")
   AC_SUBST(NETCDF_LDADD,"-lnetcdf")
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
                 src/Makefile
		 test/Makefile])
AC_OUTPUT
