//libraries
#include <glm/gtc/type_ptr.hpp>

//engine
#include <assets/textureasset.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace assets
{
  TextureAsset::TextureAsset(Json::Value value)
    : Asset(value), mTarget(GL_TEXTURE_2D), mNTextures(0), mShininess(value["shininess"].asFloat())
  {
    mImage = Magick::Image(getFilepath());
    auto specular = value["specular"];
    mSpecular = glm::vec3(specular["r"].asFloat(), specular["g"].asFloat(), specular["b"].asFloat());
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

  void TextureAsset::setProgramVariables(Program& program, const GLchar* shininess, const GLchar* specular)
  {
    auto shininessIndex = program.uniform(shininess);
    auto specularColourIndex = program.uniform(specular);

    glUniform1f(shininessIndex, mShininess);
    glUniform3fv(specularColourIndex, 1, glm::value_ptr(mSpecular));
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
    printField(where, ", specular", toString(mSpecular));
    printField(where, ", shininess", mShininess);
  }
}
