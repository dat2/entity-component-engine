#ifndef _UTILS_HPP
#define _UTILS_HPP

// libraries
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <SFML/System/Vector3.hpp>
#include <LinearMath/btVector3.h>

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
  glm::vec3 sfmlToGlm(const sf::Vector3f v);
  glm::vec3 bulletToGlm(const btVector3 v);

  btVector3 glmToBullet(const glm::vec3 v);

  std::string toString(glm::vec3 v);
  std::string toString(btVector3 v);

  std::string ensureDirectory(const std::string filepath);
}

#endif
