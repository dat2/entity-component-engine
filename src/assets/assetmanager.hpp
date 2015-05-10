#ifndef _ASSET_MANAGER_HPP
#define _ASSET_MANAGER_HPP

// libraries
#include <string>
#include <unordered_map>

// engine
#include <assets/asset.hpp>

namespace engine
{
  class Engine;
}

namespace assets
{
  class AssetManager
  {
  friend class engine::Engine;

  public:
    AssetManager(std::string baseDirectory);

    void deleteAssets();

    const std::string getBaseDirectory() const;

    template <class T>
    std::shared_ptr<T> getAsset(const std::string assetName);

    template <class T, typename ...Args>
    std::shared_ptr<T> loadAsset(Args && ...args);

  private:
    std::string mBaseDirectory;
    std::unordered_map< std::string, AssetPtr > mAssets;
  };
}

#include <assets/assetmanager.inl>

#endif
