#ifndef _ENTITY_HPP
#define _ENTITY_HPP

// standard library
#include <vector>
#include <functional>

// my own
namespace engine
{
  class Engine;
}

#include <components/component.hpp>

using namespace components;

namespace entities
{
  class Entity
  {
  friend struct std::hash<Entity>;
  friend class engine::Engine;

  public:
    Entity(engine::Engine& engine, std::string name);

    void addComponent(ComponentPtr c);
    void removeComponent(ComponentType t);

    const std::vector< ComponentPtr > getComponents() const;

    ComponentPtr getComponent(ComponentType t);

    const std::string& getName() const;

    friend bool operator==(const Entity& a, const Entity& b);
    friend std::ostream& operator<<(std::ostream& os, Entity const& e);
  private:
    engine::Engine& mEngine;
    std::string mName;
  };

  typedef std::shared_ptr<Entity> EntityPtr;
  typedef std::reference_wrapper<Entity> EntityRef;
}

namespace std
{
  template <> struct hash<entities::Entity>
  {
    size_t operator()(const entities::Entity& e) const
    {
      /* eg. return hash<int>()(x.value); */
      return hash<std::string>()(e.mName);
    }
  };
}

#endif
