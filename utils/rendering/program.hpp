#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

// standard libraries
#include <vector>

// my own
#include <utils/rendering/shader.hpp>

namespace utils
{
  class Program
  {
  public:
    Program(std::vector<Shader*> &);

    bool link(void);
    void use(void);
    void unuse(void);

    void enableAttribute(const GLchar*);
    void defineAttributeArray(const GLchar*, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);

    GLint attribute(const GLchar*) const;
    GLint uniform(const GLchar*) const;
  private:
    std::vector<Shader*> mShaders;
    GLuint mProgram;
  };
}

#endif
