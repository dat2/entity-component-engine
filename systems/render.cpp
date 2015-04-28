// opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// my own
#include <systems/render.hpp>
#include <utils/utils.hpp>

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

using namespace utils;
using namespace components;

namespace systems
{
  RenderSystem::RenderSystem(Program& program)
    : System(TRANSFORM | MODEL | TEXTURE), mProgram(program), mCamera(nullptr)
  {
  }

  void RenderSystem::updateCamera()
  {
    if(mCamera != nullptr)
    {
      mCamera->update();
      mProgram.use();

      GLint cameraIndex = mProgram.uniform("camera");
      glUniformMatrix4fv(cameraIndex, 1, GL_FALSE, glm::value_ptr(mCamera->mwv()));

      mProgram.unuse();
    }
  }

  void RenderSystem::entityChanged(Entity& entity, ComponentType newComponent)
  {
    if(mCamera == nullptr && newComponent == CAMERA)
    {
      mCamera = std::dynamic_pointer_cast<Camera>(entity.getComponent(CAMERA));
      updateCamera();
    }
  }

  void RenderSystem::run()
  {
    mProgram.use();

    if(mCamera != nullptr && mCamera->needsUpdate())
    {
      updateCamera();
    }

    for( auto &e : mEntities )
    {
      auto entity = e.get();

      auto& transform = *std::dynamic_pointer_cast<Transform>(entity.getComponent(TRANSFORM));
      auto& model = *std::dynamic_pointer_cast<Model>(entity.getComponent(MODEL));
      auto& texture = *std::dynamic_pointer_cast<Texture>(entity.getComponent(TEXTURE));

      GLint modelIndex = mProgram.uniform("model");
      glm::mat4 modelMatrix = glm::mat4();

      // rotate
      const sf::Vector3f& rotation = transform.getRotation();
      modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), X_AXIS);
      modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), Y_AXIS);
      modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), Z_AXIS);

      // scale
      modelMatrix = glm::scale(modelMatrix, sfmlToGlm(transform.getScale()));

      // translate
      modelMatrix = glm::translate(modelMatrix, sfmlToGlm(transform.getPosition()));
      glUniformMatrix4fv(modelIndex, 1, GL_FALSE, glm::value_ptr(modelMatrix));

      // set the texture
      texture.setActive(GL_TEXTURE0);
      texture.bind(0);
      texture.setUniform(mProgram);

      model.drawArrays();

      // unbind
      texture.unbind();
    }

    mProgram.unuse();
  }
}
