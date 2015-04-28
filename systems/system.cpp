#include <numeric>
#include <functional>
#include <algorithm>

#include <systems/system.hpp>

namespace systems
{
  System::System(ComponentType requiredTypes)
    : mRequiredTypes(requiredTypes)
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

    return allComponentTypes == mRequiredTypes;
  }

  void System::entityAdded(Entity& entity) { }
  void System::entityChanged(Entity& entity, ComponentType newComponent) { }
  void System::entityRemoved(Entity& entity) { }
  void System::run() { }
}
