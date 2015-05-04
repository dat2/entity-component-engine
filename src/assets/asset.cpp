//libraries

//engine
#include <assets/asset.hpp>

namespace assets
{
  Asset::Asset(Json::Value value)
    : Printable(value["name"].asString()), Named(value["name"].asString()), mFilepath(value["filepath"].asString()), mLoaded(false)
  {
  }
  void Asset::load()
  {
    mLoaded = true;
  }
  const bool Asset::isLoaded() const
  {
    return mLoaded;
  }

  const std::string& Asset::getFilepath() const
  {
    return mFilepath;
  }
}
