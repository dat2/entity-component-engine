namespace entities
{
  template <class T>
  std::shared_ptr<T> Entity::getComponent()
  {
    auto cs = getComponents();
    if(!cs)
    {
      return nullptr;
    }
    auto components = *cs;

    auto result = std::find_if(
      components.begin(), components.end(),
      [](const ComponentPtr& c)
      {
        return typeid(*c) == typeid(T);
      }
    );
    if(result == components.end())
    {
      return nullptr;
    }
    return std::dynamic_pointer_cast<T>(*result);
  }
}
