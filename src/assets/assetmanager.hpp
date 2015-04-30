#ifndef _ASSET_MANAGER_HPP
#define _ASSET_MANAGER_HPP

#include <string>
#include <unordered_map>

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

    template <class T>
    std::shared_ptr<T> getAsset(const std::string assetName)
    {
      auto search = mAssets.find(assetName);

      if(search == mAssets.end())
      {
        return nullptr;
      }

      return std::dynamic_pointer_cast<T>(search->second);
    }

    template <class T, typename ...Args>
    std::shared_ptr<T> createAsset(const std::string name, const std::string filepath, Args && ...args)
    {
      std::shared_ptr<T> ptr = std::make_shared<T>(name, mBaseDirectory + "/" + filepath, std::forward<Args>(args)...);
      mAssets.insert(std::make_pair(name, ptr));
      return ptr;
    }

  private:
    std::string mBaseDirectory;
    std::unordered_map< std::string, AssetPtr > mAssets;
  };
}

#endif
