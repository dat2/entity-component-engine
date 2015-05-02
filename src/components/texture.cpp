// standard libraries
#include <iostream>

// my own
#include <components/texture.hpp>

namespace components
{
  Texture::Texture(const std::string name)
    : Component(TEXTURE), Named(name), mAsset(nullptr)
  {
  }

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

  void Texture::setAsset(std::shared_ptr<TextureAsset> asset)
  {
    mAsset = asset;
  }

  void Texture::print(std::ostream& where) const
  {
    where << "asset=(";
    if(mAsset)
    {
      mAsset->print(where);
    }
    where << ")";
  }

}
