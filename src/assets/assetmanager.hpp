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

    void unloadAssets();

    const std::string getBaseDirectory() const;

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
    std::shared_ptr<T> loadAsset(Args && ...args)
    {
      std::shared_ptr<T> ptr = std::make_shared<T>(std::forward<Args>(args)...);
      ptr->load();

      auto name = ptr->getName();
      mAssets.insert(std::make_pair(name, ptr));

      return ptr;
    }

  private:
    std::string mBaseDirectory;
    std::unordered_map< std::string, AssetPtr > mAssets;
  };
}

#endif
