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

// std::cout << glm::to_string(sfmlToGlm(vert)) << std::endl;
namespace components
{
  Model::Model()
    : Component(MODEL), mVertexBuffer(GL_ARRAY_BUFFER), mVertexArray(), mNumVertices(0)
  {
  }

  void Model::generate()
  {
    mVertexBuffer.generate(1);
    mVertexArray.generate(1);
  }

  void Model::bind()
  {
    mVertexBuffer.bind(0);
    mVertexArray.bind(0);
  }

  void Model::loadBuffer(std::vector< sf::Vector3f > vertices, std::vector< sf::Vector2f > uvs)
  {
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

    mNumVertices = vertices.size();
  }

  void Model::unbind()
  {
    mVertexBuffer.unbind();
    mVertexArray.unbind();
  }

  /**
   * The render system must call this.
   * @param program       [description]
   * @param vertAttribute [description]
   * @param uvAttribute   [description]
   */
  void Model::prepareVertexArray(Program &program, std::string vertAttribute, std::string uvAttribute)
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

  void Model::drawArrays()
  {
    mVertexArray.bind(0);

    glDrawArrays(GL_TRIANGLES, 0, mNumVertices * 3);

    mVertexArray.unbind();
  }

  void Model::print(std::ostream& where) const
  {
    where << "Model("
      << "vertices=" << mNumVertices
      << ")";
  }
}
