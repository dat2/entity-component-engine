namespace engine
{
  template <class T, typename ...Args>
  std::shared_ptr<T> constructComponent(Args && ...args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <class T, typename ...Args>
  std::shared_ptr<T> Engine::getAsset(Args && ...args)
  {
    return mAssetManager.getAsset<T>(std::forward<Args>(args)...);
  }

  template <class T, typename ...Args>
  std::shared_ptr<T> Engine::loadAsset(Args && ...args)
  {
    return mAssetManager.loadAsset<T>(std::forward<Args>(args)...);
  }

  template <typename ...Args>
  std::shared_ptr<Entity> Engine::createEntity(const std::string name, Args && ...args)
  {
    auto result = mEntities.emplace(
      name,
      std::make_shared<Entity>(*this, name, std::forward<Args>(args)...)
    );
    mComponents.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(name),
      std::forward_as_tuple(std::make_shared< std::vector<ComponentPtr> >())
    );

    // get rid of the const of first
    return result.first->second;
  }
}
