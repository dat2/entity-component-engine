// standard libraries
#include <iostream>

// my own
#include <components/model.hpp>

namespace components
{
  Model::Model(const std::string name)
    : Component(MODEL), Named(name), mAsset(nullptr)
  {
  }

  Model::Model(Json::Value value)
    : Component(MODEL), Named(value["name"].asString()), mAsset(nullptr)
  {
  }

  void Model::draw()
  {
    if(mAsset)
    {
      mAsset->draw();
    }
  }

  void Model::setAsset(std::shared_ptr<ModelAsset> asset)
  {
    mAsset = asset;
  }

  void Model::print(std::ostream& where) const
  {
    where << "asset=(";
    if(mAsset)
    {
      mAsset->print(where);
    }
    where << ")";
  }
}
