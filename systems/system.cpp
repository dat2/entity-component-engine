#include <numeric>
#include <functional>
#include <algorithm>

#include <systems/system.hpp>

namespace systems
{
  System::System(std::string name, ComponentType requiredTypes)
    : mName(name), mRequiredTypes(requiredTypes)
  {
  }

  bool System::hasTypes(const Entity& entity) const
  {
    auto& components = entity.getComponents();

    auto allComponentTypes = EMPTY;
    for( auto &c : components )
    {
      allComponentTypes = allComponentTypes | c->getType();
    }

    return ((mRequiredTypes & allComponentTypes) == mRequiredTypes);
  }

  void System::entityAdded(Entity& entity)
  {
    std::cout << entity.getName() << " added to system " << mName << std::endl;
  }
  void System::entityChanged(Entity& entity, ComponentType newComponent) { }
  void System::entityRemoved(Entity& entity)
  {
    std::cout << entity.getName() << " removed from system " << mName << std::endl;
  }
  void System::run() { }
}
