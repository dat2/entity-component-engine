#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

// standard library
#include <vector>
#include <functional>

// my own
namespace engine
{
  class Engine;
}

#include <entities/entity.hpp>
#include <components/component.hpp>

using namespace components;
using namespace entities;

namespace systems
{
  class System
  {
  friend class engine::Engine;

  public:
    System(ComponentType requiredTypes);

    bool hasTypes(const Entity& entity) const;

    virtual void entityAdded(Entity& entity);
    virtual void entityChanged(Entity& entity, ComponentType newComponent);
    virtual void entityRemoved(Entity& entity);

    virtual void run();

  protected:
    std::vector< EntityRef > mEntities;
  private:
    ComponentType mRequiredTypes;
  };

  typedef std::shared_ptr<System> SystemPtr;
}

#endif
