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

enable_image=yes
AC_ARG_ENABLE([image],
              [AS_HELP_STRING([--disable-image], [disable wrapper for SDL2_image])])
AS_VAR_IF([enable_image], [yes], [AS_VAR_APPEND([sdl_libs], [" SDL2_image"])])
AM_CONDITIONAL([ENABLE_IMAGE], [test x$enable_image = xyes])

enable_mixer=yes
AC_ARG_ENABLE([mixer],
              [AS_HELP_STRING([--disable-mixer], [disable wrapper for SDL2_mixer])])
AS_VAR_IF([enable_mixer], [yes], [AS_VAR_APPEND([sdl_libs], [" SDL2_mixer"])])
AM_CONDITIONAL([ENABLE_MIXER], [test x$enable_mixer = xyes])

enable_ttf=yes
AC_ARG_ENABLE([ttf],
              [AS_HELP_STRING([--disable-ttf], [disable wrapper for SDL2_ttf])])
AS_VAR_IF([enable_ttf], [yes], [AS_VAR_APPEND([sdl_libs], [" SDL2_ttf"])])
AM_CONDITIONAL([ENABLE_TTF], [test x$enable_ttf = xyes])


PKG_CHECK_MODULES([SDL2], [$sdl_libs])


enable_examples=yes
AC_ARG_ENABLE([examples],
              [AS_HELP_STRING([--disable-examples], [disable building examples])])
AM_CONDITIONAL([ENABLE_EXAMPLES], [test x$enable_examples = xyes])


AC_CONFIG_FILES([Makefile
                 examples/dvd-logo/Makefile
                 examples/simple/Makefile])
AC_OUTPUT
