#ifndef _RENDER_HPP
#define _RENDER_HPP

// libraries
#include <unordered_set>
#include <unordered_map>

// engine
#include <components/common/transform.hpp>
#include <components/render/camera.hpp>
#include <components/render/model.hpp>
#include <components/render/texture.hpp>
#include <engine/engine.hpp>
#include <systems/system.hpp>
#include <systems/render/program.hpp>

using namespace components;
using namespace engine;
using namespace systems;

namespace render
{
  class RenderSystem : public System
  {
  public:
    RenderSystem();

    void updateCamera();
    void updateLights();
    void activateTexture(GLenum activeTexture, GLint texture, const std::string uniform);

    void entityAdded(Engine& engine, EntityPtr entity);
    void entityComponentAdded(Engine& engine, EntityPtr entity, ComponentType newComponent);
    void entityComponentRemoved(Engine& engine, EntityPtr entity, ComponentType newComponent);
    void entityRemoved(Engine& engine, EntityPtr entity);

    void run(Engine& engine);

  private:
    Program mProgram;

    EntityPtr mCameraEntity;
    std::vector<EntityPtr> mLightEntities;
  };
}

#endif
