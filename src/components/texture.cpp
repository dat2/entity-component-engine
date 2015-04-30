// standard libraries
#include <iostream>

// my own
#include <components/texture.hpp>

namespace components
{
  Texture::Texture(const std::string name, const std::string filepath)
    : Component(TEXTURE), Named(name), mFilepath(filepath), mAsset(nullptr)
  {
  }

  const std::string& Texture::getFilepath() const
  {
    return mFilepath;
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
