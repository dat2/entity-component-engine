// opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// my own
#include <systems/render.hpp>
#include <utils/utils.hpp>
#include <assets/modelasset.hpp>

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

using namespace assets;
using namespace utils;
using namespace components;

namespace systems
{
  RenderSystem::RenderSystem(Program& program)
    : System("render", TRANSFORM | MODEL | TEXTURE), mProgram(program), mCamera(nullptr)
  {
  }

  void RenderSystem::updateCamera()
  {
    if(mCamera != nullptr)
    {
      mProgram.use();

      mCamera->update();

      GLint cameraIndex = mProgram.uniform("camera");
      glUniformMatrix4fv(cameraIndex, 1, GL_FALSE, glm::value_ptr(mCamera->mwv()));

      mProgram.unuse();
    }
  }

  void RenderSystem::activateTexture(GLenum activeTexture, GLint texture, const std::string uniform)
  {
    glActiveTexture(activeTexture);
    glUniform1i(mProgram.uniform((GLchar *)uniform.c_str()), texture);
  }

  void RenderSystem::entityAdded(Engine& engine, Entity& entity)
  {
    System::entityAdded(engine, entity);

    // flyweight of models
    auto model = entity.getComponent<Model>(MODEL);
    auto asset = engine.getAsset<ModelAsset>(model->getName());

    if(asset == nullptr)
    {
      auto modelAsset = engine.createAsset<ModelAsset>(model->getName(), "");

      mProgram.use();

      modelAsset->generate();
      modelAsset->bind();
      modelAsset->loadBuffer(model->getVertices(), model->getUvs());
      modelAsset->prepareVAO(mProgram, "vert", "vertTexCoord");
      modelAsset->unbind();

      mProgram.unuse();

      model->setAsset(modelAsset);
    }
    else
    {
      model->setAsset(asset);
    }

    // flyweight of textures
    auto texture = entity.getComponent<Texture>(TEXTURE);
    auto texturesCheck = mTextures.find(texture->getFilename());
    if(texturesCheck == mTextures.end())
    {
      auto r = mTextures.emplace(texture->getFilename(),texture->getFilename());

      texture->generate(1);
      texture->bind(0);
      texture->setImage(r.first->second);
      texture->unbind();
    }
  }

  void RenderSystem::entityChanged(Engine& engine, Entity& entity, ComponentType newComponent)
  {
    if(mCamera == nullptr && newComponent == CAMERA)
    {
      mCamera = entity.getComponent<Camera>(CAMERA);
      updateCamera();
    }
  }

  void RenderSystem::run()
  {
    if(mCamera == nullptr)
    {
      return;
    }

    if(mCamera->needsUpdate())
    {
      updateCamera();
    }

    mProgram.use();

    for( auto &e : mEntities )
    {
      auto entity = e.get();

      auto& transform = *entity.getComponent<Transform>(TRANSFORM);
      auto& model = *entity.getComponent<Model>(MODEL);
      auto& texture = *entity.getComponent<Texture>(TEXTURE);

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
      texture.bind(0);
      activateTexture(GL_TEXTURE0, 0, "wood");

      model.draw();

      // unbind
      texture.unbind();
    }

    mProgram.unuse();
  }
}
