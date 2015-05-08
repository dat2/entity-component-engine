// libraries
#include <iostream>

// engine
#include <components/render/texture.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace render
{
  Texture::Texture(Json::Value value)
    : Component(TEXTURE), Named(value["name"].asString()), mAsset(nullptr)
  {
  }

  void Texture::bind(int n)
  {
    if(mAsset)
    {
      mAsset->bind(n);
    }
  }
  void Texture::unbind()
  {
    if(mAsset)
    {
      mAsset->unbind();
    }
  }

  void Texture::setProgramVariables(Program& program, const GLchar* shininess, const GLchar* specular)
  {
    if(mAsset)
    {
      mAsset->setProgramVariables(program, shininess, specular);
    }
  }

  void Texture::setAsset(std::shared_ptr<TextureAsset> asset)
  {
    mAsset = asset;
  }

  void Texture::print(std::ostream& where) const
  {
    where << ", asset=(";
    if(mAsset)
    {
      mAsset->print(where);
    }
    where << ")";
  }

}
