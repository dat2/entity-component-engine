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
#include <components/common/transform.hpp>
#include <components/input/controller.hpp>
#include <components/physics/rigidbody.hpp>
#include <components/render/camera.hpp>
#include <engine/engine.hpp>
#include <entities/entity.hpp>
#include <systems/input/input.hpp>
#include <systems/physics/physics.hpp>
#include <systems/render/render.hpp>
#include <utils/utils.hpp>

using namespace assets;
using namespace components;
using namespace engine;
using namespace entities;
using namespace systems;
using namespace utils;

using namespace input;
using namespace physics;
using namespace render;

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

static void FillController(const std::shared_ptr<Controller>& controller)
{
  controller->createKeyAction("moveLeft", { sf::Keyboard::Left, sf::Keyboard::A });
  controller->createKeyAction("moveRight", { sf::Keyboard::Right, sf::Keyboard::D });
  controller->createKeyAction("moveForward", { sf::Keyboard::Up, sf::Keyboard::W });
  controller->createKeyAction("moveBackward", { sf::Keyboard::Down, sf::Keyboard::S });
  controller->createKeyAction("jump", { sf::Keyboard::Space });

  controller->createKeyAction("rotateLeft", { sf::Keyboard::J });
  controller->createKeyAction("rotateRight", { sf::Keyboard::L });
  controller->createKeyAction("rotateUp", { sf::Keyboard::I });
  controller->createKeyAction("rotateDown", { sf::Keyboard::K });

  controller->createKeyAction("quit", { sf::Keyboard::Escape });
  controller->createKeyAction("refresh", { sf::Keyboard::R });

  controller->createState("refreshTime");

  // TODO tie in physics somehow
  controller->addUpdateCallback(
    [](Engine& engine, EntityPtr entity, sf::Window& window, sf::Time& time)
    {
      auto controller = entity->getComponent<Controller>();
      auto cam = entity->getComponent<Camera>();
      auto transform = entity->getComponent<Transform>();
      auto rigidbody = entity->getComponent<RigidBody>();
      if(!cam)
      {
        return;
      }

      float elapsed = time.asSeconds();

      // move the camera
      float moveForce = 200; // newtons
      float jumpForce = 1000;

      int x = controller->getKeyActionState("moveRight") - controller->getKeyActionState("moveLeft");
      glm::vec3 xForce = x * moveForce * cam->left();

      int y = controller->getKeyActionState("jump");
      glm::vec3 yForce = y * jumpForce * glm::vec3(0,1,0);

      int z = controller->getKeyActionState("moveForward") - controller->getKeyActionState("moveBackward");
      glm::vec3 zForce = z * moveForce * cam->forward();

      auto force = xForce + yForce + zForce;
      rigidbody->applyForce(force);

      // rotate the camera
      float rotateSpeed = 60.0f; // 360degrees / 3seconds;
      float rotateDist = rotateSpeed * elapsed;

      int horizontal = controller->getKeyActionState("rotateRight") - controller->getKeyActionState("rotateLeft");
      int vertical = controller->getKeyActionState("rotateUp") - controller->getKeyActionState("rotateDown");

      if(horizontal || vertical)
      {
        cam->rotate(-vertical * rotateDist, horizontal * rotateDist);
      }
    }
  );

  controller->addUpdateCallback(
    [](Engine& engine, EntityPtr entity, sf::Window& window, sf::Time& time)
    {
      auto controller = entity->getComponent<Controller>();
      if(controller->getKeyActionState("quit"))
      {
        engine.stop();
      }

      // refresh entities so we can make changes to their position and not
      // need to restart the program
      // introduce some delay into refreshing
      auto newVal = controller->getState("refreshTime") + time.asMilliseconds();
      controller->updateState("refreshTime", newVal);

      if(controller->getState("refreshTime") > 200 && controller->getKeyActionState("refresh"))
      {
        controller->updateState("refreshTime" , 0);

        engine.deleteEntities("level");
        engine.deleteAssets();

        engine.loadAssetsJson("assets.json");
        engine.loadEntitiesJson("entities.json");
        engine.updateTime();
      }
    }
  );
}

static void CreateSystems(Engine& engine, sf::Window& window)
{
  auto inputSystem = std::make_shared<InputSystem>(window);
  auto physicsSystem = std::make_shared<PhysicsSystem>();
  auto renderSystem = std::make_shared<RenderSystem>();

  engine.addSystem(inputSystem);
  engine.addSystem(physicsSystem);
  engine.addSystem(renderSystem);
}

static void AddControllerToPlayer(Engine& engine)
{
  auto e = engine.getEntity("player");
  if(e)
  {
    auto& player = *e;

    auto controller = player.getComponent<Controller>();
    FillController(controller);
  }
}

int main()
{
  sf::ContextSettings settings(24, 8, 4, 4, 1);
  sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL Entity Component System in 3D", sf::Style::Default, settings);
  window.setFramerateLimit(60);

  InitGLEW();
  InitOpenGL();

  Engine engine("resources");

  CreateSystems(engine, window);

  engine.loadAssetsJson("assets.json");
  engine.loadEntitiesJson("entities.json");

  AddControllerToPlayer(engine);

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
