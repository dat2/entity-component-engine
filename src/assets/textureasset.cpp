#include <assets/textureasset.hpp>

namespace assets
{
  TextureAsset::TextureAsset(const std::string name, const std::string filepath)
    : Asset(name, filepath), mImage(filepath), mTarget(GL_TEXTURE_2D), mNTextures(0)
  {
  }

  TextureAsset::~TextureAsset()
  {
    if(mNTextures > 0)
    {
      glDeleteTextures(mNTextures, mTextures);
      delete[] mTextures;
    }
  }

  void TextureAsset::generate(int n)
  {
    mNTextures = n;
    mTextures = new GLuint[mNTextures];

    glGenTextures(mNTextures, mTextures);
  }

  void TextureAsset::set(GLint filter, GLint wrapMode)
  {
    mImage.flip();

    Magick::Blob blob;
    mImage.magick("RGBA");
    mImage.write(&blob);

    // GL_BYTE
    glTexImage2D(mTarget, 0, GL_RGBA,
      (GLsizei)mImage.columns(), (GLsizei)mImage.rows(), 0, GL_RGBA,
      GL_UNSIGNED_BYTE, (GLubyte*)blob.data());

    // set some texture parameters
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, filter);

    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, wrapMode);
  }

  void TextureAsset::bind(int n)
  {
    glBindTexture(mTarget, mTextures[n]);
  }
  void TextureAsset::unbind()
  {
    glBindTexture(mTarget, 0);
  }
  void TextureAsset::load()
  {
    generate(1);
    bind(0);
    set();
    unbind();

    Asset::load();
  }

  const Magick::Image& TextureAsset::getImage() const
  {
    return mImage;
  }

  void TextureAsset::print(std::ostream& where) const
  {
    printField(where, "type", "TextureAsset");
    printField(where, ", name", getName());
    printField(where, ", resourcePath", getFilepath());
    printField(where, ", width", mImage.columns());
    printField(where, ", height", mImage.rows());
  }
}
