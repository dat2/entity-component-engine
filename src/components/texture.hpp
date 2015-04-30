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
    Texture(const std::string name, const std::string filepath);

    void bind(int n);
    void unbind();
    void setAsset(std::shared_ptr<TextureAsset> asset);
    void print(std::ostream& where) const;


    const std::string& getFilepath() const;
  private:
    std::string mFilepath;
    std::shared_ptr<TextureAsset> mAsset;
  };
}

#endif
