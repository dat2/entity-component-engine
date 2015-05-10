// libraries
#include <string>
#include <functional>

// engine
#include <engine/engine.hpp>
#include <entities/entity.hpp>
#include <misc/printable.hpp>

using namespace engine;

namespace entities
{
  Entity::Entity(Engine& engine, const std::string name, const std::set<std::string> tags)
    : Printable("Entity"), Named(name), mEngine(engine), mTags(std::move(tags))
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

  bool Entity::hasTag(const std::string tag) const
  {
    return std::find(mTags.begin(), mTags.end(), tag) != mTags.end();
  }

  const engine::Components Entity::getComponents() const
  {
    return mEngine.getComponents(*this);
  }

  void Entity::print(std::ostream& where) const
  {
    printField(where, "name", getName());

    auto csPtr = getComponents();
    if(!csPtr)
    {
      std::vector<ComponentPtr> components { };
      printVector(where, ", components", components);
    }
    else
    {
      auto components = *csPtr;
      printVector(where, ", components", components, [](const ComponentPtr& c) { return ToString(c->getType()); });
    }
  }

  // simplify code in engine.cpp
  Entity::Entity(engine::Engine& engine, std::string name)
    : Printable("Entity"), Named(name), mEngine(engine)
  {
  }

}
