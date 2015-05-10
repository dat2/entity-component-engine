// libraries
#include <iostream>
#include <sstream>
#include <GL/GLEW.h>
#include <glm/gtx/string_cast.hpp>

// engine
#include <utils/utils.hpp>

// http://www.lighthouse3d.com/cg-topics/error-tracking-in-opengl/
int printOglError(const char *file, int line)
{
  GLenum glErr;
  int retCode = 0;

  glErr = glGetError();
  if (glErr != GL_NO_ERROR)
  {
    // ignore deprecated warnings about gluErrorString
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

    printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));

    #pragma GCC diagnostic pop
    retCode = 1;
  }
  return retCode;
}

namespace utils
{
  glm::vec3 sfmlToGlm(const sf::Vector3f v)
  {
    return glm::vec3(v.x, v.y, v.z);
  }
  glm::vec3 bulletToGlm(const btVector3 v)
  {
    return glm::vec3(v.getX(), v.getY(), v.getZ());
  }
  btVector3 glmToBullet(const glm::vec3 v)
  {
    return btVector3(v.x, v.y, v.z);
  }

  std::string toString(glm::vec3 v)
  {
    return glm::to_string(v);
  }
  std::string toString(btVector3 v)
  {
    return toString(bulletToGlm(v));
  }

  std::string ensureDirectory(const std::string filepath)
  {
    std::string rtn = filepath;
    if(filepath.compare(filepath.length() - 1, 1, "/"))
    {
      rtn += "/";
    }
    return rtn;
  }

  glm::vec3 jsonToVector(Json::Value val, const std::string jsonMember,
    const std::string x, const std::string y, const std::string z)
  {
    auto member = val[jsonMember];
    return glm::vec3(member[x].asFloat(), member[y].asFloat(), member[z].asFloat());
  }
}
