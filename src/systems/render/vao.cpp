// engine
#include <systems/render/vao.hpp>

namespace render
{
  VertexArrayObject::VertexArrayObject()
    : mNArrays(0), mVao(NULL)
  {
  }

  VertexArrayObject::~VertexArrayObject()
  {
    if(mNArrays > 0)
    {
      glDeleteVertexArrays(mNArrays, mVao);
      delete[] mVao;
    }
  }

  void VertexArrayObject::generate(int n)
  {
    mNArrays = n;
    mVao = new GLuint[mNArrays];

    glGenVertexArrays(mNArrays,mVao);
  }

  void VertexArrayObject::bind(int n)
  {
    glBindVertexArray(mVao[n]);
  }

  void VertexArrayObject::unbind()
  {
    glBindVertexArray(0);
  }

  GLuint& VertexArrayObject::operator[](const int index)
  {
    return mVao[index];
}
}
