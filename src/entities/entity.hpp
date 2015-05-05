#ifndef _ENTITY_HPP
#define _ENTITY_HPP

// libraries
#include <functional>
#include <set>
#include <typeinfo>
#include <vector>

// engine
#include <components/component.hpp>
#include <misc/named.hpp>
#include <misc/printable.hpp>

using namespace components;

namespace engine
{
  class Engine;
  typedef std::shared_ptr< std::vector< ComponentPtr > > EntityComponentsPtr;
}

namespace entities
{
  class Entity : public Printable, public Named
  {
  friend struct std::hash<Entity>;
  friend class engine::Engine;

  public:
    Entity(engine::Engine& engine, std::string name, const std::set<std::string> tags);

    void addComponent(ComponentPtr c);
    void removeComponent(ComponentType t);
    bool hasTag(const std::string tag) const;

    const engine::EntityComponentsPtr getComponents() const;

    void print(std::ostream& where) const;

    template <class T>
    std::shared_ptr<T> getComponent()
    {
      auto csPtr = getComponents();
      if(!csPtr)
      {
        return nullptr;
      }

      auto components = *csPtr;

      auto result = std::find_if(std::begin(components), std::end(components),
        [](const ComponentPtr& c) { return typeid(*c).name() == typeid(T).name(); }
      );
      if(result != std::end(components))
      {
        return std::dynamic_pointer_cast<T>(*result);
      }
      return nullptr;
    }
  private:
    engine::Engine& mEngine;
    std::set< std::string > mTags;
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
