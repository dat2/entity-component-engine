#ifndef _RENDER_HPP
#define _RENDER_HPP

// libraries
#include <unordered_set>
#include <unordered_map>

// engine
#include <components/camera.hpp>
#include <components/transform.hpp>
#include <components/texture.hpp>
#include <components/model.hpp>
#include <engine/engine.hpp>
#include <systems/system.hpp>
#include <systems/render/program.hpp>

using namespace components;
using namespace engine;

namespace systems
{
  class RenderSystem : public System
  {
  public:
    RenderSystem();

    void updateCamera();
    void activateTexture(GLenum activeTexture, GLint texture, const std::string uniform);

    void entityAdded(Engine& engine, Entity& entity);
    void entityChanged(Engine& engine, Entity& entity, ComponentType newComponent);
    void entityRemoved(Engine& engine, Entity& entity);

    void run(Engine& engine);

  private:
    Program mProgram;
    std::shared_ptr<Camera> mCamera;
  };
}

#endif
