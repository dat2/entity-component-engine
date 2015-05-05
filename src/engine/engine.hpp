#ifndef _ENGINE_HPP
#define _ENGINE_HPP

// libraries
#include <vector>
#include <unordered_map>
#include <SFML/System/Clock.hpp>
#include <boost/optional.hpp>

// engine
#include <assets/assetmanager.hpp>
#include <components/component.hpp>
#include <entities/entity.hpp>
#include <systems/system.hpp>

using namespace entities;
using namespace components;
using namespace systems;
using namespace assets;

namespace engine
{
  typedef boost::optional< std::vector< ComponentPtr > > Components;

  class Engine
  {
  public:
    Engine(const std::string baseDirectory);

    void addSystem(SystemPtr system);

    boost::optional<Entity&> getEntity(const std::string name);

    void updateTime();
    const sf::Time& getElapsed() const;

    void addComponentToEntity(Entity &entity, ComponentPtr component);
    void removeComponentFromEntity(Entity& entity, ComponentType t);
    const Components getComponents(const Entity& entity) const;

    void run();
    void loadAssetsJson(const std::string filename);
    void loadEntitiesJson(const std::string filename);

    void clearEntities(const std::string tag);
    void unloadAssets();

    template <class T, typename ...Args>
    std::shared_ptr<T> getAsset(Args && ...args);

    template <class T, typename ...Args>
    std::shared_ptr<T> loadAsset(Args && ...args);

    template <typename ...Args>
    Entity& createEntity(Args && ...args);

  private:
    sf::Clock mClock;
    sf::Time mTime;

    AssetManager mAssetManager;
    std::vector< SystemPtr > mSystems;
    std::unordered_map< Entity, std::vector< ComponentPtr > > mEntities;
  };

  std::unordered_map< std::string, std::function< ComponentPtr(Json::Value) > > makeComponentFactory();
}

#include <engine/engine.inl>

#endif
