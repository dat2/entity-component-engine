#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

// libraries
#include <string>
#include <GL/GLEW.h>
#include <json/json.h>

// engine
#include <assets/render/textureasset.hpp>
#include <components/component.hpp>
#include <systems/render/program.hpp>

using namespace assets;
using namespace components;

namespace render
{
  class Texture : public Component, public Named
  {
  public:
    Texture(Json::Value value);

    void bind(int n);
    void unbind();

    void setProgramVariables(Program& program, const GLchar* shininess, const GLchar* specular);

    void setAsset(std::shared_ptr<TextureAsset> asset);
    void print(std::ostream& where) const;

  private:

    std::shared_ptr<TextureAsset> mAsset;
  };
}

#endif
