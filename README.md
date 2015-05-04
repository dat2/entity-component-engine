# Dependencies
1. GLEW
2. GLM
3. SFML
4. Magick++ 6.6 (Quantum 16) (headers in `/usr/local/include/ImageMagick-6/`)
5. [tinyobjloader](https://github.com/syoyo/tinyobjloader)
6. [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

GLEW, GLM, SFML, and Magick++ are expected in the default system include folders.

tinyobjloader and jsoncpp are included as submodules

# Building
Run `cmake . && make && ./openglgame` to get the engine running.
