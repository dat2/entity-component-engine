#ifndef _UTILS_HPP
#define _UTILS_HPP

// standard libraries
#include <iostream>
#include <memory>

// opengl
#include <glm/glm.hpp>

// sfml
#include <SFML/System/Vector3.hpp>

#define printGlError() printOglError(__FILE__, __LINE__)
int printOglError(const char *file, int line);

  // convert vectors between libraries
namespace utils
{
  /**
   * Convert a vector from the SFML library to a vector for the GLM library.
   * @param  v any sfml vector
   * @return   the equivalent glm vector
   */
  glm::vec3 sfmlToGlm(sf::Vector3f v);

  /**
   * Convert a vector from the GLM library to a vector for the SFML library.
   * @param  v any glm vector
   * @return   the equivalent sfml vector
   */
  sf::Vector3f glmToSfml(glm::vec3 v);

  std::string toString(sf::Vector3f v);
  std::string toString(glm::vec3 v);
}

#endif
