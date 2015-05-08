// libraries
#include <fstream>
#include <iostream>
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

namespace engine
{
  Engine::Engine(const std::string baseDirectory)
    : mAssetManager(baseDirectory)
  {
  }

  void Engine::addSystem(SystemPtr system)
  {
    mSystems.push_back(std::move(system));
  }

  boost::optional<Entity&> Engine::getEntity(const std::string name)
  {
    auto r = mEntities.find(Entity(*this, name));
    if(r != mEntities.end())
    {
      return const_cast<Entity&>(r->first);
    }

    return boost::none;
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
    auto cs = mEntities.find(entity);
    if(cs == mEntities.end())
    {
      return;
    }

    auto& components = cs->second;
    components.push_back(component);

    for( auto& system : mSystems )
    {
      if(system->hasTypes(entity))
      {
        system->mEntities.push_back(std::ref(entity));
        system->entityAdded(*this, entity);
      }
      system->entityChanged(*this, entity, component->getType());
    }
  }

  void Engine::removeComponentFromEntity(Entity& entity, ComponentType t)
  {
    auto cs = mEntities.find(entity);
    if(cs == mEntities.end())
    {
      return;
    }

    auto& components = cs->second;
    auto predicate = [&t](const ComponentPtr& c) { return c->getType() == t; };

    bool needsChanging = false;
    for(auto c : components)
    {
      needsChanging = needsChanging || predicate(c);
    }

    if(needsChanging)
    {
      for( auto& system : mSystems )
      {
        if(system->hasTypes(entity))
        {
          system->entityRemoved(*this, entity);

          auto entityPredicate = [&entity](const EntityRef& e) { return e.get().getName() == entity.getName(); };
          system->mEntities.erase(
            std::remove_if(std::begin(system->mEntities), std::end(system->mEntities), entityPredicate), std::end(system->mEntities));
        }
        system->entityChanged(*this, entity, t);
      }

      components.erase(std::remove_if(std::begin(components), std::end(components), predicate), std::end(components));
    }
  }

  const Components Engine::getComponents(const Entity& entity) const
  {
    auto cs = mEntities.find(entity);
    if(cs == mEntities.end())
    {
      return boost::none;
    }
    return cs->second;
  }

  void Engine::run()
  {
    updateTime();
    for( auto& system : mSystems )
    {
      system->run(*this);
    }
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
        std::set<std::string> tags;
        for(auto& tag : entityJson["tags"])
        {
          tags.insert(tag.asString());
        }
        auto& entity = createEntity(name, tags);

        auto components = entityJson["components"];
        for(auto& component : components)
        {
          auto type = component["type"].asString();

          entity.addComponent(constructors[type](component));
        }
      }
    }
    catch(...)
    {
    }
  }

  void Engine::clearEntities(const std::string tag)
  {
    for( auto& system : mSystems )
    {
      for(auto& kvs : mEntities)
      {
        auto& entity = const_cast<Entity&>(kvs.first);

        // for each system
        // remove all its entities that have the same tag
        auto entityPredicate = [&entity, &tag](const EntityRef eref)
        {
          auto& e = eref.get();
          return e.getName() == entity.getName() && e.hasTag(tag);
        };
        system->mEntities.erase(
          std::remove_if(std::begin(system->mEntities), std::end(system->mEntities), entityPredicate), std::end(system->mEntities));
        if(entityPredicate(entity) && system->hasTypes(entity))
        {
          system->entityRemoved(*this, entity);
        }
      }
    }

    auto it = mEntities.begin();
    while(it != mEntities.end())
    {
      if(it->first.hasTag(tag))
      {
        it = mEntities.erase(it);
      }
      else
      {
        it++;
      }
    }
    updateTime();
  }

  void Engine::unloadAssets()
  {
    mAssetManager.unloadAssets();
  }

  #define ADD_STRING(name, Name) constructors[#name] = constructComponent<Name,Json::Value>;
  std::unordered_map< std::string, std::function< ComponentPtr(Json::Value) > > makeComponentFactory()
  {
    std::unordered_map< std::string, std::function< ComponentPtr(Json::Value) > > constructors;
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
