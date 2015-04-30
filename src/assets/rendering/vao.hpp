#ifndef _VAO_HPP
#define _VAO_HPP

// opengl
#include <GL/glew.h>

namespace assets
{
  /**
   * A simple wrapper class around VAO methods in OpenGL.
   */
  class VertexArrayObject
  {
  public:
    VertexArrayObject();
    ~VertexArrayObject();

    void generate(int);

    void bind(int);
    void unbind();

    GLuint& operator[](const int index);

  private:

    int mNArrays;
    GLuint *mVao;
  };
}
#endif
