#ifndef _ENTITY_HPP
#define _ENTITY_HPP

// libraries
#include <functional>
#include <set>
#include <typeinfo>
#include <vector>
#include <boost/optional.hpp>

// engine
#include <components/component.hpp>
#include <misc/named.hpp>
#include <misc/printable.hpp>

using namespace components;

namespace engine
{
  class Engine;
  typedef std::shared_ptr< std::vector<ComponentPtr> > Components;
}

namespace entities
{
  class Entity : public Printable, public Named
  {
  friend struct std::hash<Entity>;
  friend class engine::Engine;

  public:
    Entity(engine::Engine& engine, const std::string name, const std::set<std::string> tags);

    void addComponent(ComponentPtr c);
    void removeComponent(ComponentType t);
    bool hasTag(const std::string tag) const;

    const engine::Components getComponents() const;

    void print(std::ostream& where) const;

    template <class T>
    std::shared_ptr<T> getComponent();

  protected:
    Entity(engine::Engine& engine, std::string name);
  private:
    engine::Engine& mEngine;
    std::set< std::string > mTags;
  };

  typedef std::shared_ptr<Entity> EntityPtr;
}
#include <entities/entity.inl>

#endif
