#ifndef _ASSET_HPP
#define _ASSET_HPP

#include <string>
#include <iostream>

#include <misc/printable.hpp>
#include <misc/named.hpp>

namespace assets
{

  class Asset : public Printable, public Named
  {
  public:
    Asset(const std::string name, const std::string filepath);

    const std::string& getFilepath() const;

    virtual void load();
    const bool isLoaded() const;

  private:
    std::string mFilepath;
    bool mLoaded;
  };

  typedef std::shared_ptr<Asset> AssetPtr;
}

#endif
