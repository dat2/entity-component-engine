#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

// standard libraries
#include <string>

// opengl
#include <GL/GLEW.h>

// sfml
#include <SFML/Graphics/Texture.hpp>

// imagemagick
#include <Magick++.h>

// my own
#include <components/component.hpp>
#include <utils/rendering/program.hpp>

using namespace utils;

namespace components
{
  class Texture : public Component
  {
  public:
    Texture(GLenum target, std::string uniform);
    ~Texture();

    void generate(int n);

    void setMinMagFilter(GLint filter);
    void setWrap(GLint wrapMode);
    void setImage(Magick::Image image);
    void setActive(GLenum activeTexture);
    void setUniform(Program &program);

    void bind(int n);
    void unbind();

    const GLchar *uniform() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    Magick::Image mImage;

    GLenum mTarget;
    int mNTextures;
    GLuint *mTextures;
    const GLchar *mUniform;
  };
}

#endif
