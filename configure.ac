#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([libsdl2xx], [0.1], [])
AC_CONFIG_SRCDIR([src/init.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CXX
AX_APPEND_COMPILE_FLAGS([-std=c++23], [CXX])
AC_LANG([C++])

AC_PROG_RANLIB


AS_VAR_SET([sdl_libs], [sdl2])


AC_ARG_ENABLE([image],
              [AS_HELP_STRING([--disable-image], [disable wrapper for SDL2_image])],
              [],
              [enable_image=yes])
AS_VAR_IF([enable_image], [yes], [AS_VAR_APPEND([sdl_libs], [" SDL2_image"])])
AM_CONDITIONAL([ENABLE_IMAGE], [test x$enable_image = xyes])


AC_ARG_ENABLE([mixer],
              [AS_HELP_STRING([--disable-mixer], [disable wrapper for SDL2_mixer])],
              [],
              [enable_mixer=yes])
AS_VAR_IF([enable_mixer], [yes], [AS_VAR_APPEND([sdl_libs], [" SDL2_mixer"])])
AM_CONDITIONAL([ENABLE_MIXER], [test x$enable_mixer = xyes])


AC_ARG_ENABLE([ttf],
              [AS_HELP_STRING([--disable-ttf], [disable wrapper for SDL2_ttf])],
              [],
              [enable_ttf=yes])
AS_VAR_IF([enable_ttf], [yes], [AS_VAR_APPEND([sdl_libs], [" SDL2_ttf"])])
AM_CONDITIONAL([ENABLE_TTF], [test x$enable_ttf = xyes])


PKG_CHECK_MODULES([SDL2], [$sdl_libs])


AC_ARG_ENABLE([examples],
              [AS_HELP_STRING([--enable-examples], [enable building examples])],
              [],
              [enable_examples=no])
AM_CONDITIONAL([ENABLE_EXAMPLES], [test x$enable_examples = xyes])


AC_CONFIG_FILES([Makefile
                 examples/dvd-logo/Makefile
                 examples/simple/Makefile])
AC_OUTPUT


AC_MSG_NOTICE([SDL2_image support: $enable_image])
AC_MSG_NOTICE([SDL2_mixer support: $enable_mixer])
AC_MSG_NOTICE([SDL2_ttf support:   $enable_ttf])
AC_MSG_NOTICE([Build examples:     $enable_examples])
