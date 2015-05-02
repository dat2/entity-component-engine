#include <assets/assetmanager.hpp>

namespace assets
{
  AssetManager::AssetManager(std::string baseDirectory)
    : mBaseDirectory(baseDirectory)
  {
  }

  const std::string AssetManager::getBaseDirectory() const
  {
    return mBaseDirectory;
  }
}
