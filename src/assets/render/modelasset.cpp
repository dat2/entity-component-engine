//libraries

//engine
#include <assets/render/modelasset.hpp>
#include <utils/utils.hpp>

namespace render
{
  ModelAsset::ModelAsset(Json::Value value)
    : Asset(value), mVertexBuffer(GL_ARRAY_BUFFER), mVertexArray(), mNumVertices(0)
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
  void ModelAsset::loadBuffer()
  {
    // TODO combine shape vertices?
    if(isLoaded())
    {
      std::vector<GLfloat> data;

      // (x,y,z)
      for( auto& shape : mShapes )
      {
        auto& indices = shape.mesh.indices;
        auto& vertices = shape.mesh.positions;
        auto& uvs = shape.mesh.texcoords;
        auto& normals = shape.mesh.normals;

        for(auto &i : indices)
        {
          auto x = vertices[i*3];
          auto y = vertices[i*3+1];
          auto z = vertices[i*3+2];
          data.push_back(x);
          data.push_back(y);
          data.push_back(z);

          auto u = uvs[i*2];
          auto v = uvs[i*2+1];
          data.push_back(u);
          data.push_back(v);

          auto nx = normals[i*3];
          auto ny = normals[i*3+1];
          auto nz = normals[i*3+2];
          data.push_back(nx);
          data.push_back(ny);
          data.push_back(nz);
        }
      }

      mVertexBuffer.buffer(data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
    }
  }
  void ModelAsset::prepareVAO(Program &program, const std::string vertAttribute, const std::string uvAttribute, const std::string normalAttribute)
  {
    program.use();
    bind();

    // enable the "vert" attribute of the shader
    const GLchar* vert = vertAttribute.c_str();
    program.enableAttribute(vert);
    program.defineAttributeArray(vert, 3, GL_FLOAT, GL_FALSE, (3+2+3) * sizeof(GLfloat), NULL);

    // enable the "vertTexCoord"
    const GLchar* uv = uvAttribute.c_str();
    program.enableAttribute(uv);
    program.defineAttributeArray(uv, 2, GL_FLOAT, GL_TRUE, (3+2+3) * sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));

    // enable the "vertNormal"
    const GLchar* normal = normalAttribute.c_str();
    program.enableAttribute(normal);
    program.defineAttributeArray(normal, 3, GL_FLOAT, GL_FALSE, (3+2+3) * sizeof(GLfloat), (const GLvoid*)((3+2)*sizeof(GLfloat)));

    // TODO normals
    unbind();
    program.unuse();
  }

  void ModelAsset::draw()
  {
    mVertexArray.bind(0);

    glDrawArrays(GL_TRIANGLES, 0, mNumVertices * 3);

    mVertexArray.unbind();
  }

  void ModelAsset::load()
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
        mNumVertices += s.mesh.positions.size();
      }
      mNumVertices /= 3;
    }

    Asset::load();

    generate();
    bind();
    loadBuffer();
    unbind();
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
