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
  Entity& Engine::createEntity(Args && ...args)
  {
    auto result = mEntities.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(*this, std::forward<Args>(args)...),
      std::forward_as_tuple()
    );

    // get rid of the const of first
    return const_cast<Entity&>(result.first->first);
  }
}
