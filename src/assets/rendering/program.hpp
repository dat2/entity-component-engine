#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

// standard libraries
#include <vector>

// my own
#include <assets/rendering/shader.hpp>

namespace assets
{
  class Program
  {
  public:
    Program(std::vector<ShaderPtr> shaders);

    bool link(void);
    void use(void);
    void unuse(void);

    void enableAttribute(const GLchar* attributeString);
    void defineAttributeArray(const GLchar* attributeString, GLint vertices, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

    GLint attribute(const GLchar* attribute) const;
    GLint uniform(const GLchar* uniform) const;
  private:
    std::vector<ShaderPtr> mShaders;
    GLuint mProgram;
  };
}

#endif
