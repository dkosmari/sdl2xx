AM_CPPFLAGS = \
	$(SDL2_CFLAGS) \
	-I$(top_srcdir)/include


AM_CXXFLAGS = \
	-Wall -Wextra -Werror


if ENABLE_EXAMPLES

noinst_PROGRAMS = simple


simple_SOURCES = \
	src/main.cpp


simple_LDADD = \
	$(top_builddir)/lib/libsdl2xx.la \
	$(SDL2_LIBS)

endif ENABLE_EXAMPLES
