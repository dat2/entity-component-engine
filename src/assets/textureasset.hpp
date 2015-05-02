#ifndef _TEXTURE_ASSET_HPP
#define _TEXTURE_ASSET_HPP

// opengl
#include <GL/GLEW.h>

#include <assets/asset.hpp>

// imagemagick
#include <Magick++.h>

namespace assets
{
  class TextureAsset : public Asset
  {
  public:
    TextureAsset(const std::string name, const std::string filepath);
    ~TextureAsset();

    void generate(int n);

    void set(GLint filter=GL_LINEAR, GLint wrapMode=GL_REPEAT);

    void bind(int n);
    void unbind();
    void load();

    const Magick::Image& getImage() const;
    void print(std::ostream& where) const;

  private:
    Magick::Image mImage;

    GLenum mTarget;
    int mNTextures;
    GLuint *mTextures;
  };
}
#endif
