// libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// engine
#include <assets/render/modelasset.hpp>
#include <assets/render/textureasset.hpp>
#include <components/common/transform.hpp>
#include <components/render/light.hpp>
#include <systems/render/render.hpp>
#include <utils/utils.hpp>

using namespace assets;
using namespace utils;
using namespace components;

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

namespace render
{
  RenderSystem::RenderSystem()
    : System("render", TRANSFORM | MODEL | TEXTURE), mCameraEntity(nullptr)
  {
    // shaders!
    auto vertex = Shader::fromFile("src/systems/render/shaders/vertex.shader", GL_VERTEX_SHADER);
    auto fragment = Shader::fromFile("src/systems/render/shaders/fragment.shader", GL_FRAGMENT_SHADER);

    vertex->compile();
    fragment->compile();

    auto shaders { vertex, fragment };
    mProgram.attachShaders(shaders);
    mProgram.link();

    // set gamma
    mProgram.use();
    GLint gammaIndex = mProgram.uniform("gamma");
    glUniform1f(gammaIndex, 2.2f);
    mProgram.unuse();
  }

  void RenderSystem::updateCamera()
  {
    if(mCameraEntity)
    {
      auto camera = mCameraEntity->getComponent<Camera>();
      auto transform = mCameraEntity->getComponent<Transform>();
      if(transform && (camera->isUpdated() || transform->isUpdated()) )
      {
        // set the camera matrix
        GLint cameraIndex = mProgram.uniform("camera");
        glUniformMatrix4fv(cameraIndex, 1, GL_FALSE, glm::value_ptr(camera->mwv()));

        // set the position of the camera
        GLint cameraPositionIndex = mProgram.uniform("cameraPosition");
        glUniform3fv(cameraPositionIndex, 1, glm::value_ptr(transform->position()));
      }
    }
  }

  void RenderSystem::activateTexture(GLenum activeTexture, GLint texture, const std::string uniform)
  {
    glActiveTexture(activeTexture);
    glUniform1i(mProgram.uniform((GLchar *)uniform.c_str()), texture);
  }

  void RenderSystem::entityAdded(Engine& engine, EntityPtr entity)
  {
    // model asset association
    auto model = entity->getComponent<Model>();
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

    // texture asset association
    auto texture = entity->getComponent<Texture>();
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

  void RenderSystem::entityComponentAdded(Engine& engine, EntityPtr entity, ComponentType newComponent)
  {
    if(!mCameraEntity && newComponent == CAMERA)
    {
      mCameraEntity = entity;
      updateCamera();
    }
    if(newComponent == LIGHT)
    {
      auto transform = entity-> getComponent<Transform>();
      auto light = entity->getComponent<Light>();

      mProgram.use();

      GLint lightPositionIndex = mProgram.uniform("light.position");
      glUniform3fv(lightPositionIndex, 1, glm::value_ptr(transform->position()));

      GLint intensityIndex = mProgram.uniform("light.intensities");
      glUniform3fv(intensityIndex, 1, glm::value_ptr(light->colour()));

      GLint ambientCoefficientIndex = mProgram.uniform("light.ambientCoefficient");
      glUniform1f(ambientCoefficientIndex, light->ambient());

      GLint attenuationIndex = mProgram.uniform("light.attenuationFactor");
      glUniform1f(attenuationIndex, light->attenuation());

      mProgram.unuse();
    }
  }

  void RenderSystem::entityComponentRemoved(Engine& engine, EntityPtr entity, ComponentType newComponent)
  {
    if(mCameraEntity && newComponent == CAMERA)
    {
      mCameraEntity = nullptr;
      updateCamera();
    }
  }

  void RenderSystem::entityRemoved(Engine& engine, EntityPtr entity)
  {
  }

  void RenderSystem::run(Engine& engine)
  {
    if(!mCameraEntity)
    {
      return;
    }

    mProgram.use();

    updateCamera(); // must be called within a (use / unuse block)

    for( auto &kv : mEntities )
    {
      auto entity = kv.second;

      auto transform = entity->getComponent<Transform>();
      auto model = entity->getComponent<Model>();
      auto texture = entity->getComponent<Texture>();

      // prepare matrices
      auto modelIndex = mProgram.uniform("model");
      auto normalIndex = mProgram.uniform("normalMatrix");

      auto modelMatrix = transform->matrix();
      glUniformMatrix4fv(modelIndex, 1, GL_FALSE, glm::value_ptr(modelMatrix));

      auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
      glUniformMatrix3fv(normalIndex, 1, GL_FALSE, glm::value_ptr(normalMatrix));

      texture->setProgramVariables(mProgram, "materialShininess", "materialSpecularColour");

      // set the texture
      texture->bind(0);
      activateTexture(GL_TEXTURE0, 0, "materialTexture");

      model->draw();

      texture->unbind();
    }

    mProgram.unuse();
  }
}
