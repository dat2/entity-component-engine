// open gl needs to be included very first
#include <GL/GLEW.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// sfml
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// my own
// utils
#include <utils/cube.hpp>
#include <utils/utils.hpp>

// components
#include <components/transform.hpp>
#include <components/camera.hpp>
#include <components/model.hpp>
#include <components/texture.hpp>
#include <components/controller.hpp>

// systems
#include <systems/render.hpp>
#include <systems/input.hpp>

#define ASPECT_RATIO (16.0f / 9.0f)
#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH WINDOW_HEIGHT * ASPECT_RATIO

using namespace components;
using namespace systems;

static Program *CompileProgram()
{
  // shaders!
  Shader *vertex = Shader::fromFile("resources/shaders/vertex.shader", GL_VERTEX_SHADER);
  Shader *fragment = Shader::fromFile("resources/shaders/fragment.shader", GL_FRAGMENT_SHADER);

  vertex->compile();
  fragment->compile();

  std::vector<Shader*> shaders;
  shaders.push_back(vertex);
  shaders.push_back(fragment);

  Program *program = new Program(shaders);
  program->link();

  return program;
}

static void InitGLEW()
{
  // get opengl at runtime
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK)
  {
    std::cerr << "failed to initialize glew" << std::endl;
    exit(-1);
  }
  glGetError(); // remove annoying glewExperimental error
  std::cout << "successfully initialized glew" << std::endl;

  // print out info
  std::cout << "============================================" << std::endl;
  std::cout << "Renderer: " <<  glGetString (GL_RENDERER) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL version supported " << glGetString (GL_VERSION) << std::endl;
  std::cout << "============================================" << std::endl;
}

static void InitOpenGL()
{
  // enable some opengl stuff
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

static Controller CreateController()
{
  Controller controller(1.0f, 36.0f);
  // move left
  controller.addKeyAction(
    { sf::Keyboard::Left, sf::Keyboard::A },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.move(glmToSfml(-cam.left() * c.getMoveSpeed() * elapsed.asSeconds()));
    }
  );
  // move right
  controller.addKeyAction(
    { sf::Keyboard::Right, sf::Keyboard::D },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.move(glmToSfml(cam.left() * c.getMoveSpeed() * elapsed.asSeconds()));
    }
  );
  // move fwd
  controller.addKeyAction(
    { sf::Keyboard::Up, sf::Keyboard::W },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.move(glmToSfml(cam.forward() * c.getMoveSpeed() * elapsed.asSeconds()));
    }
  );
  // move bwd
  controller.addKeyAction(
    { sf::Keyboard::Down, sf::Keyboard::S },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.move(glmToSfml(-cam.forward() * c.getMoveSpeed() * elapsed.asSeconds()));
    }
  );
  // move up
  controller.addKeyAction(
    { sf::Keyboard::Q },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.move(glmToSfml(cam.up() * c.getMoveSpeed() * elapsed.asSeconds()));
    }
  );
  // move down
  controller.addKeyAction(
    { sf::Keyboard::E },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.move(glmToSfml(-cam.up() * c.getMoveSpeed() * elapsed.asSeconds()));
    }
  );
  // rotate left
  controller.addKeyAction(
    { sf::Keyboard::J },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.rotate(0, -c.getRotateSpeed() * elapsed.asSeconds());
    }
  );
  // rotate right
  controller.addKeyAction(
    { sf::Keyboard::L },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.rotate(0, c.getRotateSpeed() * elapsed.asSeconds());
    }
  );
  // rotate up
  controller.addKeyAction(
    { sf::Keyboard::I },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.rotate(-c.getRotateSpeed() * elapsed.asSeconds(), 0);
    }
  );
  // rotate down
  controller.addKeyAction(
    { sf::Keyboard::K },
    [](Controller& c, Camera& cam, sf::Time &elapsed) {
      cam.rotate(c.getRotateSpeed() * elapsed.asSeconds(), 0);
    }
  );

  return controller;
}

int main()
{
  sf::ContextSettings settings(24, 8, 4, 4, 1);
  sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL Entity Component System in 3D", sf::Style::Default, settings);
  InitGLEW();
  InitOpenGL();

  // program
  Program program = *CompileProgram();

  // components
  Camera camera(sf::Vector3f(0, 3, 3), 45.0f, ASPECT_RATIO, 0.1f, 100.0f);
  camera.lookAt(sf::Vector3f(0, 0, 0));
  std::cout << camera << std::endl;

  Transform transform(sf::Vector3f(0,0,0), sf::Vector3f(0,0,0), sf::Vector3f(1,1,1));
  std::cout << transform << std::endl;

  Model model;
  model.generate();
  model.bind();
  model.loadBuffer(cubeVertices(), cubeUVs());
  model.prepareVertexArray(program, "vert", "vertTexCoord");
  model.unbind();
  std::cout << model << std::endl;

  Texture texture(GL_TEXTURE_2D, "wood");
  texture.generate(1);
  texture.bind(0);
  texture.setMinMagFilter(GL_LINEAR);
  texture.setWrap(GL_CLAMP_TO_EDGE);
  texture.setImage(Magick::Image("resources/images/wooden-crate.jpg"));
  texture.unbind();
  std::cout << texture << std::endl;

  Controller controller = CreateController();
  std::cout << controller << std::endl;

  // systems
  InputSystem inputSystem(camera);
  RenderSystem renderSystem(program, camera);

  while (window.isOpen())
  {
    // TODO move to input system
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }
    inputSystem.handle(controller);

    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderSystem.setCamera();
    renderSystem.renderModel(transform, model, texture);

    window.display();
  }

  return 0;
}
