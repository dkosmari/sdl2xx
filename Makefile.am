EXTRA_DIST = \
	AUTHORS \
	bootstrap \
	COPYING \
	README.md


SUBDIRS = \
	. \
	examples/dvd-logo \
	examples/simple


AM_CPPFLAGS = $(SDL2_CFLAGS) \
	-I$(srcdir)/include/sdl2xx

AM_CXXFLAGS = -Wall -Wextra -Werror

AM_LDFLAGS = $(SDL2_LIBS)


sdl2xx_HEADERS = \
	include/sdl2xx/allocator.hpp \
	include/sdl2xx/audio.hpp \
	include/sdl2xx/angle.hpp \
	include/sdl2xx/basic_wrapper.hpp \
	include/sdl2xx/blob.hpp \
	include/sdl2xx/clipboard.hpp \
	include/sdl2xx/color.hpp \
	include/sdl2xx/display.hpp \
	include/sdl2xx/endian.hpp \
	include/sdl2xx/error.hpp \
	include/sdl2xx/events.hpp \
	include/sdl2xx/game_controller.hpp \
	include/sdl2xx/gl.hpp \
	include/sdl2xx/guid.hpp \
	include/sdl2xx/init.hpp \
	include/sdl2xx/joystick.hpp \
	include/sdl2xx/mouse.hpp \
	include/sdl2xx/owner_wrapper.hpp \
	include/sdl2xx/pixels.hpp \
	include/sdl2xx/rect.hpp \
	include/sdl2xx/renderer.hpp \
	include/sdl2xx/rwops.hpp \
	include/sdl2xx/sdl.hpp \
	include/sdl2xx/sensor.hpp \
	include/sdl2xx/string.hpp \
	include/sdl2xx/surface.hpp \
	include/sdl2xx/texture.hpp \
	include/sdl2xx/unique_ptr.hpp \
	include/sdl2xx/vec2.hpp \
	include/sdl2xx/vector.hpp \
	include/sdl2xx/video.hpp \
	include/sdl2xx/window.hpp


if ENABLE_IMAGE
sdl2xx_HEADERS += include/sdl2xx/img.hpp
endif ENABLE_IMAGE

if ENABLE_MIXER
sdl2xx_HEADERS += include/sdl2xx/mix.hpp
endif ENABLE_MIXER

if ENABLE_TTF
sdl2xx_HEADERS += include/sdl2xx/ttf.hpp 
endif ENABLE_TTF


sdl2xxdir = $(includedir)/sdl2xx


lib_LIBRARIES = libsdl2xx.a

libsdl2xx_a_SOURCES = \
	src/allocator.cpp \
	src/audio.cpp \
	src/angle.cpp \
	src/blob.cpp \
	src/clipboard.cpp \
	src/color.cpp \
	src/display.cpp \
	src/error.cpp \
	src/events.cpp \
	src/game_controller.cpp \
	src/gl.cpp \
	src/guid.cpp \
	src/init.cpp \
	src/impl/utils.cpp \
	src/impl/utils.hpp \
	src/joystick.cpp \
	src/mouse.cpp \
	src/pixels.cpp \
	src/rect.cpp \
	src/renderer.cpp \
	src/rwops.cpp \
	src/sensor.cpp \
	src/surface.cpp \
	src/texture.cpp \
	src/vec2.cpp \
	src/video.cpp \
	src/window.cpp


if ENABLE_IMAGE
lib_LIBRARIES += libsdl2xx_image.a
libsdl2xx_image_a_SOURCES = src/img.cpp
endif ENABLE_IMAGE

if ENABLE_MIXER
lib_LIBRARIES += libsdl2xx_mixer.a
libsdl2xx_mixer_a_SOURCES = src/mix.cpp
endif ENABLE_MIXER

if ENABLE_TTF
lib_LIBRARIES += libsdl2xx_ttf.a
libsdl2xx_ttf_a_SOURCES = src/ttf.cpp
endif ENABLE_TTF


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> $(srcdir)/compile_flags.txt
