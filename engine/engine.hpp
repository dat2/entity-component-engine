#ifndef _ENGINE_HPP
#define _ENGINE_HPP

#include <vector>
#include <unordered_map>

#include <entities/entity.hpp>
#include <systems/system.hpp>
#include <components/component.hpp>

using namespace entities;
using namespace components;
using namespace systems;

namespace engine
{

  class Engine
  {
  public:
    Engine();

    void addSystem(SystemPtr system);

    void addComponentToEntity(Entity &entity, ComponentPtr component);
    void removeComponentFromEntity(Entity& entity, ComponentType t);
    const std::vector< ComponentPtr >& getComponents(const Entity& entity) const;

    void run();

    Entity& createEntity(const std::string& name);
  private:

    std::vector< SystemPtr > mSystems;
    std::unordered_map< Entity, std::vector<ComponentPtr> > mEntities;
  };

}

#endif
