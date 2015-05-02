#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

// standard libraries
#include <string>

// my own
#include <components/component.hpp>
#include <assets/textureasset.hpp>

using namespace assets;

namespace components
{
  class Texture : public Component, public Named
  {
  public:
    Texture(const std::string name);

    void bind(int n);
    void unbind();

    void setAsset(std::shared_ptr<TextureAsset> asset);
    void print(std::ostream& where) const;
  private:
    std::shared_ptr<TextureAsset> mAsset;
  };
}

#endif
