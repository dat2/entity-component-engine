// standard libraries
#include <iostream>

// my own
#include <utils/utils.hpp>
#include <utils/rendering/program.hpp>
#include <utils/rendering/shader.hpp>

namespace utils
{
  Program::Program(std::vector<Shader*> &shaders)
  {
    mProgram = glCreateProgram();
    mShaders = std::move(shaders);
    for(auto &shader : mShaders)
    {
      glAttachShader(mProgram, shader->getShader());
    }
  }

  bool Program::link(void)
  {
    GLint linkStatus;
    GLint logLength;
    glLinkProgram(mProgram);

    glGetProgramiv(mProgram, GL_LINK_STATUS, &linkStatus);

    if(linkStatus == GL_FALSE) {
      glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);

      GLchar *log = new GLchar[logLength + 1];
      glGetProgramInfoLog(mProgram, logLength, NULL, log);

      std::cout << "Link Error: " << log << std::endl;
    }

    return linkStatus == GL_TRUE;
  }


  void Program::use(void)
  {
    glUseProgram(mProgram);
  }

  void Program::unuse(void)
  {
    glUseProgram(0);
  }

  void Program::enableAttribute(const GLchar* attributeString)
  {
    glEnableVertexAttribArray(attribute(attributeString));
  }

  void Program::defineAttributeArray(const GLchar* attributeString, GLint vertices, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
  {
    glVertexAttribPointer(attribute(attributeString), vertices, type, normalized, stride, pointer);
    printGlError();
  }

  GLint Program::attribute(const GLchar* attribute) const
  {
    return glGetAttribLocation(mProgram, attribute);
  }

  GLint Program::uniform(const GLchar* uniform) const
  {
    return glGetUniformLocation(mProgram, uniform);
  }
}
