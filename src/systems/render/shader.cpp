// standard libraries
#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>

// my own
#include <systems/render/shader.hpp>

namespace systems
{
  ShaderPtr Shader::fromFile(std::string fileName, GLenum shaderType)
  {
    std::ifstream shaderFile(fileName);
    std::string shaderContents((std::istreambuf_iterator<char>(shaderFile)),
                   std::istreambuf_iterator<char>());
    return std::make_shared<Shader>(fileName, shaderContents, shaderType);
  }

  Shader::Shader(std::string fileName, std::string source, GLenum shaderType)
    : mFileName(fileName), mSource(source), mShaderType(shaderType)
  {
  }

  Shader::~Shader()
  {
    glDeleteShader(mShader);
  }

  Shader& Shader::operator=(const Shader &other)
  {
    mShader = other.mShader;

    return *this;
  }

  bool Shader::compile()
  {
    // variables
    GLint compileStatus;

    // allocate the shader "struct" in opengl
    mShader = glCreateShader(mShaderType);

    // set the string sources for the shader "struct" in opengl
    auto source = mSource.c_str();
    glShaderSource(mShader, 1, (GLchar**) &source, NULL);

    // compile the shader
    glCompileShader(mShader);
    glGetShaderiv(mShader, GL_COMPILE_STATUS, &compileStatus);

    // output the compile errors, if any
    if(compileStatus == GL_FALSE) {
      GLint shaderLogLength;
      glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &shaderLogLength);

      GLchar *shaderLog = new char[shaderLogLength + 1];
      glGetShaderInfoLog(mShader, shaderLogLength, NULL, shaderLog);

      std::cout << "Compile Error in file (" << mFileName << "): " << shaderLog << std::endl;
      glDeleteShader(mShader);
    }

    return compileStatus == GL_TRUE;
  }

  GLuint Shader::getShader(void) const
  {
    return mShader;
  }
}
