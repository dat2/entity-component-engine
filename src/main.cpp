//libraries
#include <fstream>
#include <GL/GLEW.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <json/json.h>

// engine
#include <assets/textureasset.hpp>
#include <assets/modelasset.hpp>
#include <components/camera.hpp>
#include <components/controller.hpp>
#include <engine/engine.hpp>
#include <entities/entity.hpp>
#include <systems/input.hpp>
#include <systems/render/program.hpp>
#include <systems/render/shader.hpp>
#include <systems/render/render.hpp>
#include <utils/utils.hpp>

using namespace assets;
using namespace components;
using namespace engine;
using namespace entities;
using namespace systems;
using namespace utils;

#define ASPECT_RATIO (16.0f / 9.0f)
#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH WINDOW_HEIGHT * ASPECT_RATIO

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

  controller->createKeyAction("moveLeft", { sf::Keyboard::Left, sf::Keyboard::A });
  controller->createKeyAction("moveRight", { sf::Keyboard::Right, sf::Keyboard::D });
  controller->createKeyAction("moveForward", { sf::Keyboard::Up, sf::Keyboard::W });
  controller->createKeyAction("moveBackward", { sf::Keyboard::Down, sf::Keyboard::S });
  controller->createKeyAction("moveUp", { sf::Keyboard::Q });
  controller->createKeyAction("moveDown", { sf::Keyboard::E });

  controller->createKeyAction("rotateLeft", { sf::Keyboard::J });
  controller->createKeyAction("rotateRight", { sf::Keyboard::L });
  controller->createKeyAction("rotateUp", { sf::Keyboard::I });
  controller->createKeyAction("rotateDown", { sf::Keyboard::K });

  controller->createKeyAction("quit", { sf::Keyboard::Escape });
  controller->createKeyAction("refresh", { sf::Keyboard::R });

  controller->createState("refreshTime");

  // TODO tie in physics somehow
  controller->addUpdateCallback(
    [](Engine& engine, Entity& entity, Controller& controller, sf::Window& window, sf::Time& time)
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

      int x = controller.getKeyActionState("moveRight") - controller.getKeyActionState("moveLeft");
      glm::vec3 xDiff = x * moveDist * cam->left();

      int y = controller.getKeyActionState("moveUp") - controller.getKeyActionState("moveDown");
      glm::vec3 yDiff = y * moveDist * cam->up();

      int z = controller.getKeyActionState("moveForward") - controller.getKeyActionState("moveBackward");
      glm::vec3 zDiff = z * moveDist * cam->forward();

      if(x || y || z)
      {
        cam->move(xDiff + yDiff + zDiff);
      }

      // rotate the camera
      float rotateSpeed = 120.0f; // 360degrees / 3seconds;
      float rotateDist = rotateSpeed * elapsed;

      int horizontal = controller.getKeyActionState("rotateRight") - controller.getKeyActionState("rotateLeft");
      int vertical = controller.getKeyActionState("rotateUp") - controller.getKeyActionState("rotateDown");

      if(horizontal || vertical)
      {
        cam->rotate(-vertical * rotateDist, horizontal * rotateDist);
      }
    }
  );

  controller->addUpdateCallback(
    [](Engine& engine, Entity& entity, Controller& controller, sf::Window& window, sf::Time& time)
    {
      if(controller.getKeyActionState("quit"))
      {
        window.close();
      }

      // refresh entities so we can make changes to their position and not
      // need to restart the program
      // introduce some delay into refreshing
      auto newVal = controller.getState("refreshTime") + time.asMilliseconds();
      controller.updateState("refreshTime", newVal);

      if(controller.getState("refreshTime") > 200 && controller.getKeyActionState("refresh"))
      {
        engine.unloadAssets();
        engine.clearEntities("level");

        engine.loadAssetsJson("assets.json");
        engine.loadEntitiesJson("entities.json");

        controller.updateState("refreshTime" , 0);
      }
    }
  );

  return controller;
}

static void CreateSystems(Engine& engine, sf::Window& window)
{
  auto inputSystem = std::make_shared<InputSystem>(window);
  auto renderSystem = std::make_shared<RenderSystem>();

  std::cout << *inputSystem << std::endl;
  std::cout << *renderSystem << std::endl;

  engine.addSystem(renderSystem);
  engine.addSystem(inputSystem);
}

static void CreatePlayer(Engine& engine)
{
  // simple player entity
  auto camera = std::make_shared<Camera>(sf::Vector3f(0, 0, 3), 45.0f, ASPECT_RATIO, 0.1f, 100.0f);
  camera->lookAt(sf::Vector3f(0, 0, 0));

  auto controller = CreateController();

  std::set<std::string> tags { "player" };
  Entity& player = engine.createEntity("player", tags);

  player.addComponent(camera);
  player.addComponent(controller);

  std::cout << player << std::endl;
}

int main()
{
  sf::ContextSettings settings(24, 8, 4, 4, 1);
  sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL Entity Component System in 3D", sf::Style::Default, settings);
  window.setFramerateLimit(60);

  InitGLEW();
  InitOpenGL();

  Engine engine("resources/");

  CreateSystems(engine, window);
  CreatePlayer(engine);

  engine.loadAssetsJson("assets.json");
  engine.loadEntitiesJson("entities.json");

  // TODO: move to window system?
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
