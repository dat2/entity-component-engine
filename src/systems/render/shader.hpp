#ifndef _SHADER_HPP
#define _SHADER_HPP

// libraries
#include <string>
#include <GL/glew.h>

namespace render
{
  class Shader;
  typedef std::shared_ptr<Shader> ShaderPtr;

  class Shader
  {
  public:
    static ShaderPtr fromFile(std::string, GLenum);

    Shader(std::string, std::string, GLenum);

    ~Shader();

    bool compile();

    Shader& operator=(const Shader &);

    GLuint getShader(void) const;
  private:
    std::string mFileName;
    std::string mSource;
    GLenum mShaderType;

    GLuint mShader;
  };
}

#endif
