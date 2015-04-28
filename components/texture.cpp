// standard libraries
#include <iostream>

// my own
#include <components/texture.hpp>

namespace components
{
  Texture::Texture(GLenum target, std::string uniform)
    : Component(TEXTURE), mTarget(target), mNTextures(0), mUniform(uniform.c_str())
  {
  }

  Texture::~Texture()
  {
    if(mNTextures > 0)
    {
      glDeleteTextures(mNTextures, mTextures);
      delete[] mTextures;
    }
  }

  void Texture::generate(int n)
  {
    mNTextures = n;
    mTextures = new GLuint[mNTextures];

    glGenTextures(mNTextures, mTextures);
  }
  void Texture::setImage(Magick::Image image, GLint filter, GLint wrapMode)
  {
    mImage = image;
    mImage.flip();

    Magick::Blob blob;
    mImage.magick("RGBA");
    mImage.write(&blob);

    // GL_BYTE
    glTexImage2D(mTarget, 0, GL_RGBA,
      (GLsizei)image.columns(), (GLsizei)image.rows(), 0, GL_RGBA,
      GL_UNSIGNED_BYTE, (GLubyte*)blob.data());

    // set some texture parameters
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, filter);

    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, wrapMode);
  }
  void Texture::setActive(GLenum activeTexture)
  {
    glActiveTexture(activeTexture);
  }
  void Texture::setUniform(Program &program)
  {
    glUniform1i(program.uniform(uniform()), 0);
  }

  void Texture::bind(int n)
  {
    glBindTexture(mTarget, mTextures[n]);
  }
  void Texture::unbind()
  {
    glBindTexture(mTarget, 0);
  }

  const GLchar *Texture::uniform() const
  {
    return mUniform;
  }

  void Texture::print(std::ostream& where) const
  {
    where << "Texture("
      << "width=" << mImage.columns()
      << ", height=" << mImage.rows()
      << ", uniform=" << mUniform
      << ")";
  }
}
