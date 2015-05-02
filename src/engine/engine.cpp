#include <unordered_map>
#include <iostream>

#include <json/json.h>
#include <jsoncpp.cpp>
#include <fstream>

#include <engine/engine.hpp>
#include <assets/modelasset.hpp>
#include <assets/textureasset.hpp>

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
    auto& components = mEntities.at(entity);
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
    auto& components = mEntities.at(entity);
    auto predicate = [&t](const ComponentPtr& c) { return c->getType() == t; };
    components.erase(std::remove_if(std::begin(components), std::end(components), predicate), std::end(components));

    for( auto& system : mSystems )
    {
      if(!system->hasTypes(entity))
      {
        system->mEntities.push_back(std::ref(entity));
        auto entityPredicate = [&entity](const EntityRef& e) { return e.get().getName() == entity.getName(); };
        system->mEntities.erase(
          std::remove_if(std::begin(system->mEntities), std::end(system->mEntities), entityPredicate), std::end(system->mEntities));
        system->entityRemoved(*this, entity);
      }
      system->entityChanged(*this, entity, t);
    }
  }

  const std::vector< ComponentPtr >& Engine::getComponents(const Entity& entity) const
  {
    return mEntities.at(entity);
  }

  Entity& Engine::createEntity(const std::string& name)
  {
    auto result = mEntities.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(*this, name),
      std::forward_as_tuple()
    );

    // get rid of the const of first
    return const_cast<Entity&>(result.first->first);
  }

  void Engine::run()
  {
    for( auto& system : mSystems )
    {
      system->run();
    }
  }

  void Engine::loadJson(std::string filename)
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
    catch(Json::RuntimeError e)
    {
    }
  }

}
