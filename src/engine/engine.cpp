// libraries
#include <fstream>
#include <iostream>
#include <numeric>
#include <functional>
#include <json/json.h>

// engine
#include <assets/render/modelasset.hpp>
#include <assets/render/textureasset.hpp>
#include <components/common/transform.hpp>
#include <components/input/controller.hpp>
#include <components/physics/rigidbody.hpp>
#include <components/render/camera.hpp>
#include <components/render/light.hpp>
#include <components/render/model.hpp>
#include <components/render/texture.hpp>
#include <engine/engine.hpp>
#include <utils/utils.hpp>

using namespace utils;

using namespace input;
using namespace physics;
using namespace render;

using namespace std::placeholders;

namespace engine
{
  Engine::Engine(const std::string baseDirectory)
    : mStopped(false), mAssetManager(baseDirectory)
  {
  }

  void Engine::addSystem(SystemPtr system)
  {
    mSystems.push_back(std::move(system));
  }

  std::shared_ptr<Entity> Engine::getEntity(const std::string name)
  {
    auto r = mEntities.find(name);
    if(r != mEntities.end())
    {
      return (r->second);
    }
    return nullptr;
  }

  void Engine::updateTime()
  {
    mTime = mClock.restart();
  }
  const sf::Time& Engine::getElapsed() const
  {
    return mTime;
  }

  void Engine::addComponentToEntity(Entity &entity, ComponentPtr component)
  {
    auto cs = mComponents.find(entity.getName());
    if(cs == mComponents.end())
    {
      return;
    }

    auto& components = cs->second;
    components->push_back(component);
    auto entityptr = mEntities[entity.getName()];

    for( auto& system : mSystems )
    {
      if(system->hasTypes(entity))
      {
        system->mEntities[entity.getName()] = mEntities[entity.getName()];
        system->entityAdded(*this, entityptr);
      }
      system->entityComponentAdded(*this, entityptr, component->getType());
    }
  }

  void Engine::removeComponentFromEntity(Entity& entity, ComponentType t)
  {
    // find the components vector
    auto cs = mComponents.find(entity.getName());
    if(cs == mComponents.end())
    {
      return;
    }
    auto& components = cs->second;
    auto entityptr = mEntities[entity.getName()];

    // check if the components vector contains a component with the type requested
    auto isType = [&t](const ComponentPtr& c) { return c->getType() == t; };
    bool hasComponentType = std::accumulate(components->begin(), components->end(),
      false, [&isType](const bool h, const ComponentPtr& c) { return h || isType(c); });

    // if the components vector has the type, then remove it from the vector
    if(hasComponentType)
    {
      std::vector<SystemPtr> entitySystems;
      for( auto& system : mSystems )
      {
        system->entityComponentRemoved(*this, entityptr, t);
        // if the entity is a part of the system, we may need to remove it from the system
        if(system->hasTypes(entity))
        {
          entitySystems.push_back(system);
        }
      }

      components->erase(std::remove_if(components->begin(), components->end(), isType), components->end());

      for( auto& system : entitySystems )
      {
        // if, after removing the entity, the entity is no longer valid for the system
        // remove it from the system
        if(!system->hasTypes(entity))
        {
          // if the entity can no longer be in the system, remove it
          // call lifecycle method, in case system needs to prepare
          system->entityRemoved(*this, entityptr);

          // remove entity from system
          system->mEntities.erase(entity.getName());
        }
      }
    }
  }

  const Components Engine::getComponents(const Entity& entity) const
  {
    auto cs = mComponents.find(entity.getName());
    if(cs == mComponents.end())
    {
      return nullptr;
    }
    return cs->second;
  }

  void Engine::run()
  {
    if(!mStopped)
    {
      updateTime();
      for( auto& system : mSystems )
      {
        system->run(*this);
      }
    }

    if(mStopped)
    {
      deleteAssets();
      deleteEntities();
      deleteSystems();
    }
  }

  void Engine::stop()
  {
    mStopped = true;
  }

  void Engine::loadAssetsJson(const std::string filename)
  {
    Json::Value root;
    std::ifstream assets(mAssetManager.getBaseDirectory() + filename);
    try
    {
      assets >> root;

      // load all model assets
      auto models = root["models"];
      auto modelsBasepath = ensureDirectory(mAssetManager.getBaseDirectory() + models["basepath"].asString());
      for( auto& modelJson : models["assets"])
      {
        modelJson["filepath"] = Json::Value(modelsBasepath + modelJson["filepath"].asString());
        loadAsset<ModelAsset>(modelJson);
      }

      // load all texture assets
      auto textures = root["textures"];
      auto texturesBasepath = ensureDirectory(mAssetManager.getBaseDirectory() + textures["basepath"].asString());
      for( auto& textureJson : textures["assets"])
      {
        textureJson["filepath"] = Json::Value(texturesBasepath + textureJson["filepath"].asString());
        loadAsset<TextureAsset>(textureJson);
      }
    }
    catch(...)
    {
    }
  }

  void Engine::loadEntitiesJson(const std::string filename)
  {
    Json::Value root;
    std::ifstream entitiesFile(mAssetManager.getBaseDirectory() + filename);

    auto constructors = makeComponentFactory();

    try
    {
      entitiesFile >> root;

      auto entities = root["entities"];
      for(auto& entityJson : entities)
      {
        auto name = entityJson["name"].asString();
        // we can't reload entities we already have
        if(mEntities.find(name) != mEntities.end())
        {
          continue;
        }

        std::set<std::string> tags;
        for(auto& tag : entityJson["tags"])
        {
          tags.insert(tag.asString());
        }
        auto entity = createEntity(name, tags);

        auto components = entityJson["components"];
        for(auto& component : components)
        {
          auto type = component["type"].asString();

          entity->addComponent(constructors[type](component, entity));
        }
      }
    }
    catch(...)
    {
    }
  }

  void Engine::deleteAssets()
  {
    mAssetManager.deleteAssets();
  }

  void Engine::deleteEntities(const std::string tag)
  {
    bool deleteAll = tag.empty();

    for( auto it = mEntities.begin(); it != mEntities.end() ; )
    {
      auto& entity = it->second;
      bool deleteEntity = deleteAll || entity->hasTag(tag);

      if(deleteEntity)
      {
        // remove all entity's components
        auto cs = *entity->getComponents();
        std::vector<ComponentType> types;
        for(auto& c : cs)
        {
          types.push_back(c->getType());
        }

        for(auto& t : types)
        {
          entity->removeComponent(t);
        }

        it = mEntities.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }
  void Engine::deleteSystems()
  {
    mSystems.clear();
  }

  #define ADD_STRING(name, Name) constructors[#name] = constructComponent<Name,Json::Value,EntityPtr>;
  std::unordered_map< std::string, std::function< ComponentPtr(Json::Value,EntityPtr) > > makeComponentFactory()
  {
    std::unordered_map< std::string, std::function< ComponentPtr(Json::Value,EntityPtr) > > constructors;
    ADD_STRING(model,Model);
    ADD_STRING(texture,Texture);
    ADD_STRING(transform,Transform);
    ADD_STRING(light,Light);
    ADD_STRING(camera,Camera);
    ADD_STRING(controller,Controller);
    ADD_STRING(physics,RigidBody);

    return constructors;
  }
}
