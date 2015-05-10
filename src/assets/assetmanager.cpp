//libraries

//engine
#include <assets/assetmanager.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace assets
{
  AssetManager::AssetManager(std::string baseDirectory)
    : mBaseDirectory(ensureDirectory(baseDirectory))
  {
  }

  void AssetManager::deleteAssets()
  {
    mAssets.clear();
  }

  const std::string AssetManager::getBaseDirectory() const
  {
    return mBaseDirectory;
  }
}
