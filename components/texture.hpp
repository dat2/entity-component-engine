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
    Texture(GLenum target, std::string filename);
    ~Texture();

    void generate(int n);

    void setImage(Magick::Image image, GLint filter=GL_LINEAR, GLint wrapMode=GL_REPEAT);

    void bind(int n);
    void unbind();

    const std::string& getFilename() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    std::string mFilename;
    Magick::Image mImage;

    GLenum mTarget;
    int mNTextures;
    GLuint *mTextures;
  };
}

#endif
