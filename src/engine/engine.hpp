#ifndef _ENGINE_HPP
#define _ENGINE_HPP

#include <vector>
#include <unordered_map>

#include <entities/entity.hpp>
#include <systems/system.hpp>
#include <components/component.hpp>
#include <assets/assetmanager.hpp>

using namespace entities;
using namespace components;
using namespace systems;
using namespace assets;

namespace engine
{

  class Engine
  {
  public:
    Engine(const std::string baseDirectory);

    void addSystem(SystemPtr system);

    void addComponentToEntity(Entity &entity, ComponentPtr component);
    void removeComponentFromEntity(Entity& entity, ComponentType t);
    const std::vector< ComponentPtr >& getComponents(const Entity& entity) const;

    void run();
    void loadJson(std::string filename);

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

    Entity& createEntity(const std::string& name);
  private:

    AssetManager mAssetManager;
    std::vector< SystemPtr > mSystems;
    std::unordered_map< Entity, std::vector<ComponentPtr> > mEntities;
  };

}

#endif
