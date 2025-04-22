AM_CPPFLAGS = \
	$(SDL2_CFLAGS) \
	-I$(top_srcdir)/include


AM_CXXFLAGS = \
	-Wall -Wextra -Werror


noinst_PROGRAMS = simple


simple_SOURCES = \
	src/main.cpp


simple_LDADD = \
	$(top_builddir)/libsdl2xx.a \
	$(SDL2_LIBS)
