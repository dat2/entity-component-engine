// standard libraries
#include <vector>
#include <iostream>

// glm
#include <glm/gtx/string_cast.hpp>

// sfml
#include <SFML/System/Vector3.hpp>

// my own
#include <components/model.hpp>
#include <utils/utils.hpp>

namespace components
{
  Model::Model(const std::string name, std::vector< sf::Vector3f > vertices, std::vector< sf::Vector2f > uvs)
    : Component(MODEL), Named(name), mAsset(nullptr), mVertices(std::move(vertices)), mUvs(std::move(uvs))
  {
  }

  void Model::draw()
  {
    if(mAsset != nullptr)
    {
      mAsset->draw();
    }
  }

  void Model::setAsset(std::shared_ptr<ModelAsset> asset)
  {
    mAsset = asset;
  }

  const std::vector< sf::Vector3f > Model::getVertices() const
  {
    return mVertices;
  }

  const std::vector< sf::Vector2f > Model::getUvs() const
  {
    return mUvs;
  }

  void Model::print(std::ostream& where) const
  {
    where << "asset=(";
    if(mAsset != nullptr)
    {
      mAsset->print(where);
    }
    where << ")";
  }
}
