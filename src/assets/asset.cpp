#include <assets/asset.hpp>

namespace assets
{
  Asset::Asset(const std::string name, const std::string filepath)
    : Printable(name), Named(name), mFilepath(filepath)
  {
  }

  const std::string& Asset::getFilepath() const
  {
    return mFilepath;
  }
}
