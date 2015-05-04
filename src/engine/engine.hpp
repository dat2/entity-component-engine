#ifndef _ENGINE_HPP
#define _ENGINE_HPP

// libraries
#include <vector>
#include <unordered_map>

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
  typedef std::shared_ptr< std::vector< ComponentPtr > > EntityComponentsPtr;

  class Engine
  {
  public:
    Engine(const std::string baseDirectory);

    void addSystem(SystemPtr system);

    void addComponentToEntity(Entity &entity, ComponentPtr component);
    void removeComponentFromEntity(Entity& entity, ComponentType t);
    const EntityComponentsPtr getComponents(const Entity& entity) const;

    void run();
    void loadAssetsJson(const std::string filename);
    void loadEntitiesJson(const std::string filename);

    void clearEntities(const std::string tag);
    void unloadAssets();

    template <class T, typename ...Args>
    std::shared_ptr<T> getAsset(Args && ...args)
    {
      return mAssetManager.getAsset<T>(std::forward<Args>(args)...);
    }

    template <class T, typename ...Args>
    std::shared_ptr<T> loadAsset(Args && ...args)
    {
      return mAssetManager.loadAsset<T>(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    Entity& createEntity(Args && ...args)
    {
      auto result = mEntities.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(*this, std::forward<Args>(args)...),
        std::forward_as_tuple(std::make_shared< std::vector<ComponentPtr> >())
      );

      // get rid of the const of first
      return const_cast<Entity&>(result.first->first);
    }

  private:

    AssetManager mAssetManager;
    std::vector< SystemPtr > mSystems;
    std::unordered_map< Entity, EntityComponentsPtr > mEntities;
  };

}

#endif
