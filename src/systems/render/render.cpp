// opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// my own
#include <utils/utils.hpp>
#include <systems/render/render.hpp>

#include <assets/modelasset.hpp>
#include <assets/textureasset.hpp>

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

using namespace assets;
using namespace utils;
using namespace components;

namespace systems
{
  RenderSystem::RenderSystem()
    : System("render", TRANSFORM | MODEL | TEXTURE), mCamera(nullptr)
  {
    // shaders!
    auto vertex = Shader::fromFile("src/systems/render/shaders/vertex.shader", GL_VERTEX_SHADER);
    auto fragment = Shader::fromFile("src/systems/render/shaders/fragment.shader", GL_FRAGMENT_SHADER);

    vertex->compile();
    fragment->compile();

    auto shaders { vertex, fragment };
    mProgram.attachShaders(shaders);
    mProgram.link();
  }

  void RenderSystem::updateCamera()
  {
    if(mCamera)
    {
      mProgram.use();

      mCamera->update();

      // set the camera matrix
      GLint cameraIndex = mProgram.uniform("camera");
      glUniformMatrix4fv(cameraIndex, 1, GL_FALSE, glm::value_ptr(mCamera->mwv()));

      // set the light to be the position of the camera
      GLint positionIndex = mProgram.uniform("light.position");
      glUniform3fv(positionIndex, 1, glm::value_ptr(mCamera->position()));

      // set the light colour
      GLint intensityIndex = mProgram.uniform("light.intensities");
      glUniform3fv(intensityIndex, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

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
    auto modelAsset = engine.getAsset<ModelAsset>(model->getName());

    if(modelAsset)
    {
      modelAsset->prepareVAO(mProgram, "vert", "vertTexCoord", "vertNormal");

      model->setAsset(modelAsset);
    }
    else
    {
      std::cerr << "Model " << model->getName() << " was not loaded yet!" << std::endl;
    }

    // flyweight of textures
    auto texture = entity.getComponent<Texture>(TEXTURE);
    if(texture)
    {
      auto textureAsset = engine.getAsset<TextureAsset>(texture->getName());

      if(textureAsset)
      {
        texture->setAsset(textureAsset);
      }
      else
      {
        std::cerr << "Texture " << texture->getName() << " was not loaded yet!" << std::endl;
      }
    }
  }

  void RenderSystem::entityChanged(Engine& engine, Entity& entity, ComponentType newComponent)
  {
    if(!mCamera && newComponent == CAMERA)
    {
      mCamera = entity.getComponent<Camera>(CAMERA);
      updateCamera();
    }
  }

  void RenderSystem::entityRemoved(Engine& engine, Entity& entity)
  {
    if( entity.getComponent<Camera>(CAMERA) == mCamera)
    {
      mCamera = nullptr;
    }
  }

  void RenderSystem::run(Engine& engine)
  {
    if(!mCamera)
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

      auto transform = entity.getComponent<Transform>(TRANSFORM);
      auto model = entity.getComponent<Model>(MODEL);
      auto texture = entity.getComponent<Texture>(TEXTURE);

      auto modelIndex = mProgram.uniform("model");
      auto normalIndex = mProgram.uniform("normalMatrix");

      auto modelMatrix = transform->matrix();
      glUniformMatrix4fv(modelIndex, 1, GL_FALSE, glm::value_ptr(modelMatrix));

      auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
      glUniformMatrix3fv(normalIndex, 1, GL_FALSE, glm::value_ptr(normalMatrix));

      // set the texture
      texture->bind(0);
      activateTexture(GL_TEXTURE0, 0, "materialTexture");

      model->draw();

      texture->unbind();
    }

    mProgram.unuse();
  }
}
