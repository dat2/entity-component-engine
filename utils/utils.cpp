// standard libraries
#include <iostream>

// opengl
#include <GL/GLEW.h>

// my own
#include <utils/utils.hpp>

// http://www.lighthouse3d.com/cg-topics/error-tracking-in-opengl/
int printOglError(char *file, int line)
{
  GLenum glErr;
  int    retCode = 0;

  glErr = glGetError();
  if (glErr != GL_NO_ERROR)
  {
    printf("glError in file %s @ line %d: %s\n",
       file, line, gluErrorString(glErr));
    retCode = 1;
  }
  return retCode;
}

namespace utils
{
  glm::vec3 sfmlToGlm(sf::Vector3f v)
  {
    return glm::vec3(v.x, v.y, v.z);
  }

  sf::Vector3f glmToSfml(glm::vec3 v)
  {
    return sf::Vector3f(v.x, v.y, v.z);
  }

  std::ostream& printVector(std::ostream& os, sf::Vector3f& v)
  {
    return os << "(" << v.x << "," << v.y << "," << v.z << ")";
  }
}
