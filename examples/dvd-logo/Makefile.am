EXTRA_DIST = assets


AM_CPPFLAGS = \
	$(SDL2_CFLAGS) \
	-I$(top_srcdir)/include


AM_CXXFLAGS = \
	-Wall -Wextra -Werror


if ENABLE_EXAMPLES

noinst_PROGRAMS = dvd-logo


dvd_logo_SOURCES = \
	src/main.cpp


dvd_logo_LDADD = \
	$(top_builddir)/libsdl2xx_image.a \
	$(top_builddir)/libsdl2xx_mixer.a \
	$(top_builddir)/libsdl2xx_ttf.a \
	$(top_builddir)/libsdl2xx.a \
	$(SDL2_LIBS)

endif ENABLE_EXAMPLES
