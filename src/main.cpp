//libraries
#include <fstream>
#include <GL/GLEW.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <json/json.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>

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

static void FillController(const std::shared_ptr<Controller>& controller)
{
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
      auto cam = entity.getComponent<Camera>();
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
}

static void CreateSystems(Engine& engine, sf::Window& window)
{
  auto inputSystem = std::make_shared<InputSystem>(window);
  auto renderSystem = std::make_shared<RenderSystem>();

  engine.addSystem(renderSystem);
  engine.addSystem(inputSystem);
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

  // build the broadphase algorithm
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();

  // build the collision config and dispatcher
  btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

  // build the physics solver
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  // make the world
  btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  dynamicsWorld->setGravity(btVector3(0, -10, 0));

  // do stuff in the world

  // create ground plane
  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

  btDefaultMotionState* groundMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

  btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

  dynamicsWorld->addRigidBody(groundRigidBody);

  // create falling ball
  btCollisionShape* fallShape = new btSphereShape(1);

  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

  btScalar mass = 1;
  btVector3 fallInertia(0, 0, 0);
  fallShape->calculateLocalInertia(mass, fallInertia);

  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
  btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);

  dynamicsWorld->addRigidBody(fallRigidBody);

  // update world

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
    dynamicsWorld->stepSimulation(engine.getElapsed().asSeconds(), 10);

    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);

    auto o = trans.getOrigin();

    auto e = engine.getEntity("tree");
    if(e)
    {
      auto entity = *e;
      auto t = entity.getComponent<Transform>();
      if(t)
      {
        t->setPosition(sf::Vector3f(o.getX(), o.getY(), o.getZ()));
      }
    }
    // std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;

    window.display();
  }

  // Clean up behind ourselves like good little programmers
  dynamicsWorld->removeRigidBody(fallRigidBody);
  delete fallRigidBody->getMotionState();
  delete fallRigidBody;

  dynamicsWorld->removeRigidBody(groundRigidBody);
  delete groundRigidBody->getMotionState();
  delete groundRigidBody;

  delete fallShape;
  delete groundShape;

  delete dynamicsWorld;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;

  return 0;
}
