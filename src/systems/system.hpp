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
#include <misc/named.hpp>
#include <misc/printable.hpp>

using namespace components;
using namespace entities;

namespace systems
{
  class System : public Printable, public Named
  {
  friend class engine::Engine;

  public:
    System(std::string name, ComponentType requiredTypes);

    bool hasTypes(const Entity& entity) const;

    virtual void entityAdded(engine::Engine& engine, Entity& entity);
    virtual void entityChanged(engine::Engine& engine, Entity& entity, ComponentType newComponent);
    virtual void entityRemoved(engine::Engine& engine, Entity& entity);

    virtual void run();

    void print(std::ostream& where) const;

  protected:
    std::vector< EntityRef > mEntities;
  private:
    ComponentType mRequiredTypes;
  };

  typedef std::shared_ptr<System> SystemPtr;
}

#endif
