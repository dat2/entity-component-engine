#ifndef _MODEL_ASSET_HPP
#define _MODEL_ASSET_HPP

// libraries
#include <json/json.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <tiny_obj_loader.h>

// engine
#include <assets/asset.hpp>
#include <systems/render/vao.hpp>
#include <systems/render/vbo.hpp>
#include <systems/render/program.hpp>

using namespace assets;

namespace render
{

  class ModelAsset : public Asset
  {
  public:
    ModelAsset(Json::Value value);

    void generate();
    void bind();
    void unbind();

    void loadBuffer();
    void prepareVAO(Program &program, const std::string vertAttribute, const std::string uvAttribute, const std::string normalAttribute);

    void draw();
    void load();

    const int getNumVertices() const;
    void print(std::ostream& where) const;
  private:
    VertexBufferObject mVertexBuffer;
    VertexArrayObject mVertexArray;

    std::vector<tinyobj::shape_t> mShapes;
    std::vector<tinyobj::material_t> mMaterials;

    int mNumVertices;
  };

}

#endif
