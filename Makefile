CC = g++
CFLAGS = -Wall -std=c++11
SRCS = $(shell find . -name '*.cpp')
OBJ = $(SRCS:cpp=o)

ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
INCLUDE = -I/usr/local/include/ImageMagick-6/ -I$(ROOT_DIR)/
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL -lGLEW -lMagick++-6.Q16.6 -lMagickCore-6.Q16 -lm -ldl

MAGICK_CONFIG = -DMAGICKCORE_HDRI_ENABLE=false -DMAGICKCORE_QUANTUM_DEPTH=16
MAGICK_SCRIPT = `Magick++-config --cppflags --cxxflags --ldflags --libs`

%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) $(MAGICK_CONFIG) -c -o $@ $<

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(MAGICK_SCRIPT)

clean:
	rm -f $(OBJ)
