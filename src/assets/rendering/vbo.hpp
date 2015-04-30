#ifndef _VBO_HPP
#define _VBO_HPP

// opengl
#include <GL/glew.h>

namespace assets
{
  /**
   * A simple wrapper class around VBO's in OpenGL
   */
  class VertexBufferObject
  {
  public:
    VertexBufferObject(const GLenum);
    ~VertexBufferObject();

    void generate(int);

    void bind(int);
    void unbind();

    void buffer(GLsizeiptr, const GLvoid *, GLenum);

    GLenum getTarget() const;

    GLuint& operator[](const int index);

  private:

    const GLenum mTarget;
    int mNBuffers;
    GLuint *mVbo;
  };
}

#endif
