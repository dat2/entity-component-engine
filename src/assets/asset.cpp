#include <assets/asset.hpp>

namespace assets
{
  Asset::Asset(const std::string name, const std::string filepath)
    : Printable(name), Named(name), mFilepath(filepath), mLoaded(false)
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
