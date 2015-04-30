#include <string>
#include <functional>

#include <misc/printable.hpp>
#include <engine/engine.hpp>
#include <entities/entity.hpp>

using namespace engine;

namespace entities
{
  Entity::Entity(Engine& engine, std::string name)
    : Printable("Entity"), Named(name), mEngine(engine)
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

  const std::vector< ComponentPtr >& Entity::getComponents() const
  {
    return mEngine.getComponents(*this);
  }

  void Entity::print(std::ostream& where) const
  {
    printField(where, "name", getName());
    printVector(where, ", components", getComponents(), [](const ComponentPtr& c) { return ToString(c->getType()); });
  }

}
