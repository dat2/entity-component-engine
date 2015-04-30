#ifndef _MODEL_HPP
#define _MODEL_HPP

// standard libraries
#include <vector>
#include <iostream>

// sfml
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

// my own
#include <components/component.hpp>
#include <assets/modelasset.hpp>

using namespace assets;

namespace components
{
  class Model : public Component, public Named
  {
  public:
    Model(const std::string name, std::vector< sf::Vector3f > vertices, std::vector< sf::Vector2f > uvs);

    void draw();
    void setAsset(std::shared_ptr<ModelAsset> asset);

    const std::vector< sf::Vector3f > getVertices() const;
    const std::vector< sf::Vector2f > getUvs() const;

    void print(std::ostream& where) const;
  private:
    std::shared_ptr<ModelAsset> mAsset;
    std::vector< sf::Vector3f > mVertices;
    std::vector< sf::Vector2f > mUvs;
  };
}

#endif
