#include <iostream>

#include <json/json.h>
#include <fstream>

#include <engine/engine.hpp>

#include <assets/modelasset.hpp>
#include <assets/textureasset.hpp>

#include <components/model.hpp>
#include <components/texture.hpp>
#include <components/transform.hpp>

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

  void Engine::addComponentToEntity(Entity &entity, ComponentPtr component)
  {
    auto csPtr = mEntities.at(entity);
    auto& components = *csPtr;
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
    auto& csPtr = mEntities.at(entity);
    if(!csPtr)
    {
      return;
    }

    auto components = *csPtr;
    auto predicate = [&t](const ComponentPtr& c) { return c->getType() == t; };
    components.erase(std::remove_if(std::begin(components), std::end(components), predicate), std::end(components));

    for( auto& system : mSystems )
    {
      if(!system->hasTypes(entity))
      {
        auto entityPredicate = [&entity](const EntityRef& e) { return e.get().getName() == entity.getName(); };
        system->mEntities.erase(
          std::remove_if(std::begin(system->mEntities), std::end(system->mEntities), entityPredicate), std::end(system->mEntities));
        system->entityRemoved(*this, entity);
      }
      system->entityChanged(*this, entity, t);
    }
  }

  const EntityComponentsPtr Engine::getComponents(const Entity& entity) const
  {
    try
    {
      return mEntities.at(entity);
    }
    catch(...)
    {
      return nullptr;
    }
  }

  void Engine::run()
  {
    for( auto& system : mSystems )
    {
      system->run(*this);
    }
  }

  void Engine::loadAssetsJson(const std::string filename)
  {
    Json::Value root;
    std::ifstream assets(mAssetManager.getBaseDirectory() + "/" + filename);
    try
    {
      assets >> root;

      // load all model assets
      auto models = root["models"];
      auto modelsBasepath = models["basepath"].asString();
      for( auto& model : models["assets"])
      {
        auto name = model["name"].asString();
        auto filepath = modelsBasepath + model["filepath"].asString();
        loadAsset<ModelAsset>(name, filepath);
      }

      // load all texture assets
      auto textures = root["textures"];
      auto texturesBasepath = textures["basepath"].asString();
      for( auto& texture : textures["assets"])
      {
        auto name = texture["name"].asString();
        auto filepath = texturesBasepath + texture["filepath"].asString();
        loadAsset<TextureAsset>(name, filepath);
      }
    }
    catch(...)
    {
    }
  }

  void Engine::loadEntitiesJson(const std::string filename)
  {
    Json::Value root;
    std::ifstream entitiesFile(mAssetManager.getBaseDirectory() + "/" + filename);
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

          // todo if statements are disgusting
          if(type == "model")
          {
            entity.addComponent(std::make_shared<Model>(component));
          }
          else if(type == "texture")
          {
            entity.addComponent(std::make_shared<Texture>(component));
          }
          else if(type == "transform")
          {
            entity.addComponent(std::make_shared<Transform>(component));
          }
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

        auto entityPredicate = [&entity, &tag](const EntityRef& e)
        {
          auto entity = e.get();
          return entity.getName() == entity.getName() && entity.hasTag(tag);
        };
        system->mEntities.erase(
          std::remove_if(std::begin(system->mEntities), std::end(system->mEntities), entityPredicate), std::end(system->mEntities));
        if(entityPredicate(entity))
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
  }

  void Engine::unloadAssets()
  {
    mAssetManager.unloadAssets();
  }

}
