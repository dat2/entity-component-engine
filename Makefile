# compiler options
CC = g++
CFLAGS = -Wall -std=c++11 -g

# allows infinitely recursive directories
SRCS = $(shell find . -name '*.cpp')
OBJ = $(SRCS:cpp=o)

# this allows us to use <> for files starting from the root directory
ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
INCLUDE = -I/usr/local/include/ImageMagick-6/ -I$(ROOT_DIR)/

# sfml, glew, imagemagick, math, dynamic lib
LIBS = -lsfml-window -lsfml-system -lGLEW -lMagick++-6.Q16.6 -lMagickCore-6.Q16 -lm -ldl -framework OpenGL

# image magick requires some build configuration
MAGICK_CONFIG = -DMAGICKCORE_HDRI_ENABLE=false -DMAGICKCORE_QUANTUM_DEPTH=16
MAGICK_SCRIPT = `Magick++-config --cppflags --cxxflags --ldflags --libs`


# main depends on all objects
openglgame: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(MAGICK_SCRIPT)

# copied from
# http://stackoverflow.com/questions/2394609/makefile-header-dependencies
# easier dependencies management
depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(INCLUDE) $(CFLAGS) $(MAGICK_CONFIG) -MM $^ > ./.depend;

include .depend

# all cpp files get an object file
%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) $(MAGICK_CONFIG) -c -o $@ $<

# clean deletes all objects
clean:
	rm -f $(OBJ)
