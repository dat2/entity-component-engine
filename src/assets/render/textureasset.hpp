#ifndef _TEXTURE_ASSET_HPP
#define _TEXTURE_ASSET_HPP

// libraries
#include <GL/GLEW.h>
#include <glm/vec3.hpp>
#include <json/json.h>
#include <Magick++.h>

//engine
#include <assets/asset.hpp>
#include <systems/render/program.hpp>

using namespace assets;

namespace render
{
  class TextureAsset : public Asset
  {
  public:
    TextureAsset(Json::Value value);
    ~TextureAsset();

    void generate(int n);

    void set(GLint filter=GL_LINEAR, GLint wrapMode=GL_REPEAT);

    void bind(int n);
    void unbind();
    void load();

    void setProgramVariables(Program& program, const GLchar* shininess, const GLchar* specular);

    const Magick::Image& getImage() const;
    void print(std::ostream& where) const;

  private:
    Magick::Image mImage;

    GLenum mTarget;
    int mNTextures;
    GLuint *mTextures;

    float mShininess;
    glm::vec3 mSpecular;
  };
}
#endif
