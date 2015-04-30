#include <assets/modelasset.hpp>
#include <utils/utils.hpp>

namespace assets
{
  ModelAsset::ModelAsset(const std::string name, const std::string filepath)
    : Asset(name, filepath), mVertexBuffer(GL_ARRAY_BUFFER), mVertexArray(), mNumVertices(0), mLoaded(false)
  {
    auto path = Asset::getFilepath().c_str();

    std::string err = tinyobj::LoadObj(mShapes, mMaterials, path);

    if (!err.empty()) {
      std::cerr << "Warning: Couldn't load " << getName() << "(" << path << "): " << err << std::endl;
    }

    if(mShapes.size())
    {
      for(auto& s : mShapes)
      {
        mNumVertices += s.mesh.positions.size() / 3;
      }
      mLoaded = true;
    }
  }

  void ModelAsset::generate()
  {
    mVertexBuffer.generate(1);
    mVertexArray.generate(1);
  }
  void ModelAsset::bind()
  {
    mVertexBuffer.bind(0);
    mVertexArray.bind(0);
  }
  void ModelAsset::unbind()
  {
    mVertexBuffer.unbind();
    mVertexArray.unbind();
  }

  void ModelAsset::loadBuffer(const std::vector< sf::Vector3f > vertices, const std::vector< sf::Vector2f > uvs)
  {
    mNumVertices = vertices.size();
    std::vector<GLfloat> data;
    for( auto &vert : vertices )
    {
      data.push_back(vert.x);
      data.push_back(vert.y);
      data.push_back(vert.z);
    }
    for( auto &uv : uvs )
    {
      data.push_back(uv.x);
      data.push_back(uv.y);
    }
    mVertexBuffer.buffer(data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
  }
  void ModelAsset::loadBuffer()
  {
    // TODO combine shape vertices?
    if(mLoaded)
    {
      std::vector<GLfloat> data;

      for( auto& shape : mShapes )
      {
        auto& vertices = shape.mesh.positions;
        auto& indices = shape.mesh.indices;
        for(auto &i : indices)
        {
          auto x = vertices[i*3];
          auto y = vertices[i*3+1];
          auto z = vertices[i*3+2];
          data.push_back(x);
          data.push_back(y);
          data.push_back(z);
        }
      }

      mVertexBuffer.buffer(data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
    }
  }
  void ModelAsset::prepareVAO(Program &program, const std::string vertAttribute, const std::string uvAttribute)
  {
    // enable the "vert" attribute of the shader
    const GLchar* vert = vertAttribute.c_str();
    program.enableAttribute(vert);
    program.defineAttributeArray(vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // enable the "vertTexCoord"
    const GLchar* uv = uvAttribute.c_str();
    program.enableAttribute(uv);
    program.defineAttributeArray(uv, 2, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)(mNumVertices*3*sizeof(GLfloat)));

    // TODO normals
  }

  void ModelAsset::draw()
  {
    mVertexArray.bind(0);

    glDrawArrays(GL_TRIANGLES, 0, mNumVertices * 3);

    mVertexArray.unbind();
  }

  const int ModelAsset::getNumVertices() const
  {
    return mNumVertices;
  }
  const bool ModelAsset::isLoaded() const
  {
    return mLoaded;
  }

  void ModelAsset::print(std::ostream& where) const
  {
    printField(where, "type", "ModelAsset");
    printField(where, ", name", getName());
    printField(where, ", resourcePath", getFilepath());
    printField(where, ", vertices", mNumVertices);
  }
}
