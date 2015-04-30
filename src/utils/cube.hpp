#ifndef _CUBE_HPP
#define _CUBE_HPP

// standard libraries
#include <vector>

// sfml
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

namespace utils
{
  std::vector< sf::Vector3f > cubeVertices();
  std::vector< sf::Vector2f > cubeUVs();
}

#endif
