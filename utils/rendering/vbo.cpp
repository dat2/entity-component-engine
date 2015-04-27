// my own
#include <utils/rendering/vbo.hpp>

namespace utils
{
  VertexBufferObject::VertexBufferObject(const GLenum target)
    : mTarget(target), mNBuffers(0), mVbo(NULL)
  {
  }

  VertexBufferObject::~VertexBufferObject()
  {
    if(mNBuffers > 0){
      glDeleteBuffers(mNBuffers, mVbo);
      delete[] mVbo;
    }
  }

  void VertexBufferObject::generate(int n)
  {
    mNBuffers = n;
    mVbo = new GLuint[mNBuffers];

    glGenBuffers(mNBuffers, mVbo);
  }

  void VertexBufferObject::bind(int n)
  {
    glBindBuffer(mTarget, mVbo[n]);
  }

  void VertexBufferObject::unbind()
  {
    glBindBuffer(mTarget, 0);
  }

  void VertexBufferObject::buffer(GLsizeiptr size, const GLvoid *data, GLenum usage)
  {
    glBufferData(mTarget, size, data, usage);
  }

  GLenum VertexBufferObject::getTarget() const
  {
    return mTarget;
  }

  GLuint& VertexBufferObject::operator[](const int index)
  {
    return mVbo[index];
  }
}
