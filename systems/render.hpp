#ifndef _RENDER_HPP
#define _RENDER_HPP

// my own
// utils
#include <utils/rendering/program.hpp>

// components
#include <components/camera.hpp>
#include <components/transform.hpp>
#include <components/texture.hpp>
#include <components/model.hpp>

// systems
#include <systems/system.hpp>

using namespace utils;
using namespace components;

namespace systems
{
  class RenderSystem : public System
  {
  public:
    RenderSystem(Program& program, Camera& camera);

    void setCamera();

    void renderModel(Transform &transform, Model &model, Texture &texture);

  private:
    Program& mProgram;
    Camera& mCamera;
  };
}

#endif
