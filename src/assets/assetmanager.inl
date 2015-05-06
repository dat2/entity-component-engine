namespace assets
{
  template <class T>
  std::shared_ptr<T> AssetManager::getAsset(const std::string assetName)
  {
    auto search = mAssets.find(assetName);

    if(search == mAssets.end())
    {
      return nullptr;
    }

    return std::dynamic_pointer_cast<T>(search->second);
  }

  template <class T, typename ...Args>
  std::shared_ptr<T> AssetManager::loadAsset(Args && ...args)
  {
    std::shared_ptr<T> ptr = std::make_shared<T>(std::forward<Args>(args)...);
    ptr->load();

    auto name = ptr->getName();
    mAssets.insert(std::make_pair(name, ptr));

    return ptr;
  }
}
