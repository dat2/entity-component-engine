#include <assets/modelasset.hpp>

namespace assets
{
  ModelAsset::ModelAsset(const std::string name, const std::string filepath)
    : Asset(name, filepath), mVertexBuffer(GL_ARRAY_BUFFER), mVertexArray(), mNumVertices(0)
  {
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

  void ModelAsset::print(std::ostream& where) const
  {
    printField(where, "type", "ModelAsset");
    printField(where, ", name", getName());
    printField(where, ", resourcePath", getFilepath());
    printField(where, ", vertices", mNumVertices);
  }
}
