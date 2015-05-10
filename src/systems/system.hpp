#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

// libraries
#include <unordered_map>
#include <functional>
#include <vector>

// engine
#include <components/component.hpp>
#include <entities/entity.hpp>
#include <misc/named.hpp>
#include <misc/printable.hpp>

using namespace components;
using namespace entities;

namespace engine
{
  class Engine;
}

namespace systems
{
  class System : public Printable, public Named
  {
  friend class engine::Engine;

  public:
    System(std::string name, ComponentType requiredTypes);

    bool hasTypes(const Entity& entity) const;

    virtual void entityAdded(engine::Engine& engine, Entity& entity);
    virtual void entityComponentAdded(engine::Engine& engine, Entity& entity, ComponentType newComponent);
    virtual void entityComponentRemoved(engine::Engine& engine, Entity& entity, ComponentType newComponent);
    virtual void entityRemoved(engine::Engine& engine, Entity& entity);

    virtual void run(engine::Engine& engine);

    void print(std::ostream& where) const;

  protected:
    std::unordered_map< std::string, std::shared_ptr<Entity> > mEntities;
  private:
    ComponentType mRequiredTypes;
  };

  typedef std::shared_ptr<System> SystemPtr;
}

#endif
