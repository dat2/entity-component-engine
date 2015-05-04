//libraries

//engine
#include <assets/assetmanager.hpp>

namespace assets
{
  AssetManager::AssetManager(std::string baseDirectory)
    : mBaseDirectory(baseDirectory)
  {
  }

  void AssetManager::unloadAssets()
  {
    mAssets.clear();
  }

  const std::string AssetManager::getBaseDirectory() const
  {
    return mBaseDirectory;
  }
}
