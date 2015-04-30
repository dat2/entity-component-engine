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

#include <misc/printable.hpp>
#include <misc/named.hpp>

#include <components/component.hpp>

using namespace components;

namespace entities
{
  class Entity : public Printable, public Named
  {
  friend struct std::hash<Entity>;
  friend class engine::Engine;

  public:
    Entity(engine::Engine& engine, std::string name);

    void addComponent(ComponentPtr c);
    void removeComponent(ComponentType t);

    const std::vector< ComponentPtr >& getComponents() const;

    void print(std::ostream& where) const;

    template <class T>
    std::shared_ptr<T> getComponent(ComponentType t)
    {
      auto& components = getComponents();
      auto result = std::find_if(std::begin(components), std::end(components),
        [&t](const ComponentPtr& c) { return c->getType() == t; }
      );
      if(result != std::end(components))
      {
        return std::dynamic_pointer_cast<T>(*result);
      }
      return nullptr;
    }
  private:
    engine::Engine& mEngine;
  };

  typedef std::shared_ptr<Entity> EntityPtr;
  typedef std::reference_wrapper<Entity> EntityRef;
}

namespace std
{
  template <> struct hash<entities::Entity> : hash<Named>
  {
  };
}
#endif
