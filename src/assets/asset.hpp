#ifndef _ASSET_HPP
#define _ASSET_HPP

// libraries
#include <iostream>
#include <string>
#include <json/json.h>

//engine
#include <misc/named.hpp>
#include <misc/printable.hpp>

namespace assets
{

  class Asset : public Printable, public Named
  {
  public:
    Asset(Json::Value value);

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
