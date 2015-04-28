// standard libraries
#include <iostream>

// my own
#include <components/texture.hpp>

namespace components
{
  Texture::Texture(GLenum target, std::string filename)
    : Component(TEXTURE), mFilename(filename), mTarget(target), mNTextures(0)
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

  void Texture::bind(int n)
  {
    glBindTexture(mTarget, mTextures[n]);
  }
  void Texture::unbind()
  {
    glBindTexture(mTarget, 0);
  }

  const std::string& Texture::getFilename() const
  {
    return mFilename;
  }

  void Texture::print(std::ostream& where) const
  {
    where << "Texture("
      << "width=" << mImage.columns()
      << ", height=" << mImage.rows()
      << ")";
  }
}
