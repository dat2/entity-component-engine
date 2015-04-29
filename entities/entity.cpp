#include <algorithm>

#include <engine/engine.hpp>
#include <entities/entity.hpp>

using namespace engine;

namespace entities
{
  Entity::Entity(Engine& engine, std::string name)
    : mEngine(engine), mName(name)
  {
  }

  void Entity::addComponent(ComponentPtr c)
  {
    mEngine.addComponentToEntity(*this, c);
  }

  void Entity::removeComponent(ComponentType t)
  {
    mEngine.removeComponentFromEntity(*this, t);
  }

  const std::vector< ComponentPtr > Entity::getComponents() const
  {
    return mEngine.getComponents(*this);
  }

  const std::string& Entity::getName() const
  {
    return mName;
  }

  bool operator==(const Entity& a, const Entity& b)
  {
    return a.mName == b.mName;
  }

  std::ostream& operator<<(std::ostream& os, Entity const& e)
  {
    os << "Entity("
       << "name=" << e.mName
       << ", components=(";

    // print each component, with an ',' after it
    auto& components = e.getComponents();
    if(components.size() >= 1)
    {
      std::for_each(std::begin(components), std::end(components) - 1,
        [&](const ComponentPtr& c) { os << ToString(c->getType()) << ","; });

      // except for the last element
      auto& last = components.back();
      os << ToString(last->getType());
    }

    return os << ")" << ")";
  }
}
