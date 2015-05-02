cmake_minimum_required (VERSION 3.1)
project (entitycomponentengine)

find_package(ImageMagick COMPONENTS Magick++)

include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(libs/tinyobjloader)
include_directories(libs/jsoncpp/dist)

include_directories("${PROJECT_SOURCE_DIR}/src")

find_library(TINYOBJLIB NAMES tinyobjloader
  PATHS ${PROJECT_SOURCE_DIR}/libs/tinyobjloader NO_DEFAULT_PATH)
# find_library(JSONCPPLIB NAMES jsoncpp
  # PATHS ${PROJECT_SOURCE_DIR}/libs/jsoncpp/src/lib_json NO_DEFAULT_PATH)

file(GLOB_RECURSE openglgame_SRCS
    "src/*.hpp"
    "src/*.cpp"
)

add_executable(openglgame ${openglgame_SRCS})
target_compile_options(openglgame PUBLIC -Wall -std=c++11 -g -DMAGICKCORE_HDRI_ENABLE=false -DMAGICKCORE_QUANTUM_DEPTH=16)
target_link_libraries(openglgame sfml-window sfml-system GLEW m dl ${ImageMagick_LIBRARIES} ${TINYOBJLIB} ${JSONCPPLIB} "-framework OpenGL")

install(TARGETS openglgame DESTINATION bin)
