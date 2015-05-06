cmake_minimum_required (VERSION 3.1)
project (entitycomponentengine)

find_package(ImageMagick COMPONENTS Magick++)
find_package(Bullet)

include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(libs/tinyobjloader)
include_directories(libs/jsoncpp/dist)

include_directories("${PROJECT_SOURCE_DIR}/src")

find_library(TINYOBJLIB NAMES tinyobjloader
  PATHS ${PROJECT_SOURCE_DIR}/libs/tinyobjloader NO_DEFAULT_PATH)

file(GLOB_RECURSE openglgame_SRCS
  "src/*.hpp"
  "src/*.cpp"
)

add_executable(openglgame ${openglgame_SRCS})
target_compile_options(openglgame PUBLIC -Wall -std=c++1y -g -DMAGICKCORE_HDRI_ENABLE=false -DMAGICKCORE_QUANTUM_DEPTH=16)
target_link_libraries(openglgame sfml-window sfml-system GLEW m dl ${ImageMagick_LIBRARIES} ${TINYOBJLIB} ${JSONCPPLIB} ${BULLET_LIBRARIES} "-framework OpenGL")

install(TARGETS openglgame DESTINATION bin)
