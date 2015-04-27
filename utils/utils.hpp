#ifndef _UTILS_HPP
#define _UTILS_HPP

// standard libraries
#include <iostream>

// opengl
#include <glm/glm.hpp>

// sfml
#include <SFML/System/Vector3.hpp>

#define printGlError() printOglError(__FILE__, __LINE__)
int printOglError(char *file, int line);

// convert sfml vectors to glm vectors
namespace utils
{
  glm::vec3 sfmlToGlm(sf::Vector3f v);
  sf::Vector3f glmToSfml(glm::vec3 v);

  std::ostream& printVector(std::ostream& os, sf::Vector3f& v);
}

#endif
