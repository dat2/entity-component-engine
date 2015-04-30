# Dependencies
1. Magick++ 6.6 (Quantum 16) (headers in `/usr/local/include/ImageMagick-6/`)
2. GLEW
3. GLM
4. SFML
5. [tinyobjloader](https://github.com/syoyo/tinyobjloader) included as a submodule

# Building
Currently only works on mac, due to the "-framework OpenGL" flag.
Run `make && ./openglgame` to get the engine running.
