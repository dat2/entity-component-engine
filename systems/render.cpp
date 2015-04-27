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
  RenderSystem::RenderSystem(Program& program, Camera& camera)
    : System(TRANSFORM | MODEL | TEXTURE), mProgram(program), mCamera(camera)
  {
  }

  void RenderSystem::setCamera()
  {
    mProgram.use();

    GLint cameraIndex = mProgram.uniform("camera");
    glUniformMatrix4fv(cameraIndex, 1, GL_FALSE, glm::value_ptr(mCamera.mwv()));

    mProgram.unuse();
  }

  void RenderSystem::renderModel(Transform &transform, Model &model, Texture &texture)
  {
    mProgram.use();

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

    mProgram.unuse();
  }
}
