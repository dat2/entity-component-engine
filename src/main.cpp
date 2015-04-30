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

// engine
#include <engine/engine.hpp>

// entities
#include <entities/entity.hpp>

// components
#include <components/transform.hpp>
#include <components/camera.hpp>
#include <components/model.hpp>
#include <components/texture.hpp>
#include <components/controller.hpp>

// systems
#include <systems/render.hpp>
#include <systems/input.hpp>

#include <assets/rendering/program.hpp>
#include <assets/rendering/shader.hpp>

#define ASPECT_RATIO (16.0f / 9.0f)
#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH WINDOW_HEIGHT * ASPECT_RATIO

using namespace components;
using namespace systems;
using namespace entities;
using namespace engine;
using namespace utils;
using namespace assets;

static std::shared_ptr<Program> CompileProgram()
{
  // shaders!
  auto vertex = Shader::fromFile("resources/shaders/vertex.shader", GL_VERTEX_SHADER);
  auto fragment = Shader::fromFile("resources/shaders/fragment.shader", GL_FRAGMENT_SHADER);

  vertex->compile();
  fragment->compile();

  auto shaders { vertex, fragment };
  auto program = std::make_shared<Program>(shaders);
  program->link();

  return program;
}

static void PrintGLInfo()
{
  // print out info
  std::cout << "============================================" << std::endl;
  std::cout << "Renderer: " <<  glGetString (GL_RENDERER) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL version supported " << glGetString (GL_VERSION) << std::endl;
  std::cout << "============================================" << std::endl;
}

static void InitGLEW()
{
  // get opengl at runtime
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK)
  {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    exit(-1);
  }
  glGetError(); // remove annoying glewExperimental error
  std::cout << "Initialized GLEW" << std::endl;

  PrintGLInfo();
}

static void InitOpenGL()
{
  // enable some opengl stuff
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

static ComponentPtr CreateController()
{
  auto controller = std::make_shared<Controller>();

  controller->createKeyboardAction("moveLeft", { sf::Keyboard::Left, sf::Keyboard::A });
  controller->createKeyboardAction("moveRight", { sf::Keyboard::Right, sf::Keyboard::D });
  controller->createKeyboardAction("moveForward", { sf::Keyboard::Up, sf::Keyboard::W });
  controller->createKeyboardAction("moveBackward", { sf::Keyboard::Down, sf::Keyboard::S });
  controller->createKeyboardAction("moveUp", { sf::Keyboard::Q });
  controller->createKeyboardAction("moveDown", { sf::Keyboard::E });

  controller->createKeyboardAction("rotateLeft", { sf::Keyboard::J });
  controller->createKeyboardAction("rotateRight", { sf::Keyboard::L });
  controller->createKeyboardAction("rotateUp", { sf::Keyboard::I });
  controller->createKeyboardAction("rotateDown", { sf::Keyboard::K });

  controller->createKeyboardAction("quit", { sf::Keyboard::Escape });

  // TODO tie in physics somehow
  controller->addUpdateCallback(
    [](Entity& entity, Controller& controller, sf::Window& window, sf::Time& time)
    {
      auto cam = entity.getComponent<Camera>(CAMERA);
      if(cam == nullptr)
      {
        return;
      }

      float elapsed = time.asSeconds();

      // move the camera
      float moveSpeed = 3.0; // units/s
      float moveDist = moveSpeed * elapsed; // m/s * s == m

      int x = controller.getActionState("moveRight") - controller.getActionState("moveLeft");
      glm::vec3 xDiff = x * moveDist * cam->left();

      int y = controller.getActionState("moveUp") - controller.getActionState("moveDown");
      glm::vec3 yDiff = y * moveDist * cam->up();

      int z = controller.getActionState("moveForward") - controller.getActionState("moveBackward");
      glm::vec3 zDiff = z * moveDist * cam->forward();

      if(x || y || z)
      {
        cam->move(xDiff + yDiff + zDiff);
      }

      // rotate the camera
      float rotateSpeed = 120.0f; // 360degrees / 3seconds;
      float rotateDist = rotateSpeed * elapsed;

      int horizontal = controller.getActionState("rotateRight") - controller.getActionState("rotateLeft");
      int vertical = controller.getActionState("rotateUp") - controller.getActionState("rotateDown");

      if(horizontal || vertical)
      {
        cam->rotate(-vertical * rotateDist, horizontal * rotateDist);
      }
    }
  );

  controller->addUpdateCallback(
    [](Entity& entity, Controller& controller, sf::Window& window, sf::Time& time)
    {
      if(controller.getActionState("quit"))
      {
        window.close();
      }
    }
  );

  return controller;
}

static void CreateSystems(Engine& engine, Program& program, sf::Window& window)
{
  auto inputSystem = std::make_shared<InputSystem>(window);
  auto renderSystem = std::make_shared<RenderSystem>(program);

  std::cout << *inputSystem << std::endl;
  std::cout << *renderSystem << std::endl;

  engine.addSystem(renderSystem);
  engine.addSystem(inputSystem);
}

static void CreatePlayer(Engine& engine)
{
  // simple player entity
  auto camera = std::make_shared<Camera>(sf::Vector3f(0, 3, 3), 45.0f, ASPECT_RATIO, 0.1f, 100.0f);
  camera->lookAt(sf::Vector3f(0, 0, 0));

  auto controller = CreateController();

  Entity& player = engine.createEntity("player");

  player.addComponent(camera);
  player.addComponent(controller);

  std::cout << player << std::endl;
}

static void CreateEntities(Engine& engine, Program& program)
{
  // shared variables
  auto model = std::make_shared<Model>("woodCube", cubeVertices(), cubeUVs());
  auto model2 = std::make_shared<Model>("woodCube", cubeVertices(), cubeUVs());
  auto texture = std::make_shared<Texture>("woodenCrate", "images/wooden-crate.jpg");

  // box testing
  auto transform = std::make_shared<Transform>(sf::Vector3f(0,0,0), sf::Vector3f(0,0,0), sf::Vector3f(1,1,1));
  auto transform2 = std::make_shared<Transform>(sf::Vector3f(0,2,0), sf::Vector3f(0,0,0), sf::Vector3f(1,1,1));

  auto& box = engine.createEntity("box");
  box.addComponent(transform);
  box.addComponent(model);
  box.addComponent(texture);
  std::cout << box << std::endl;

  auto& box2 = engine.createEntity("box2");
  box2.addComponent(transform2);
  box2.addComponent(model2);
  box2.addComponent(texture);
  std::cout << box2 << std::endl;
}

int main()
{
  sf::ContextSettings settings(24, 8, 4, 4, 1);
  sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL Entity Component System in 3D", sf::Style::Default, settings);
  InitGLEW();
  InitOpenGL();

  // program
  Program program = *CompileProgram();

  Engine engine("resources");

  CreateSystems(engine, program, window);
  CreatePlayer(engine);
  CreateEntities(engine, program);

  // std::cout << player << std::endl;

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
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    engine.run();

    window.display();
  }

  return 0;
}
