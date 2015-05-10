#ifndef _UTILS_HPP
#define _UTILS_HPP

// libraries
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <SFML/System/Vector3.hpp>
#include <json/json.h>
#include <LinearMath/btVector3.h>

#define printGlError() printOglError(__FILE__, __LINE__)
int printOglError(const char *file, int line);

  // convert vectors between libraries
namespace utils
{
  glm::vec3 sfmlToGlm(const sf::Vector3f v);
  glm::vec3 bulletToGlm(const btVector3 v);

  btVector3 glmToBullet(const glm::vec3 v);

  std::string toString(glm::vec3 v);
  std::string toString(btVector3 v);

  std::string ensureDirectory(const std::string filepath);

  glm::vec3 jsonToVector(Json::Value val, const std::string jsonMember,
    const std::string x="x", const std::string y="y", const std::string z="z");
}

#endif
