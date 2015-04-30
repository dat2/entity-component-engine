#ifndef _RENDER_HPP
#define _RENDER_HPP

#include <unordered_set>
#include <unordered_map>

// my own
// utils
#include <engine/engine.hpp>
#include <assets/rendering/program.hpp>

// components
#include <components/camera.hpp>
#include <components/transform.hpp>
#include <components/texture.hpp>
#include <components/model.hpp>

// systems
#include <systems/system.hpp>

using namespace assets;
using namespace components;
using namespace engine;

namespace systems
{
  class RenderSystem : public System
  {
  public:
    RenderSystem(Program& program);

    void updateCamera();
    void activateTexture(GLenum activeTexture, GLint texture, const std::string uniform);

    void entityAdded(Engine& engine, Entity& entity);
    void entityChanged(Engine& engine, Entity& entity, ComponentType newComponent);
    void run();

  private:
    Program& mProgram;
    std::shared_ptr<Camera> mCamera;
  };
}

#endif
