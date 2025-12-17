#include "Game.h"

#include <iostream>
#include <fstream>
#include "Components.hpp"
#include "Vec2.hpp"

Game::Game(const std::string &configFile)
  : m_text(m_font, "Default", 24)
{

  init(configFile);
}

void Game::init(const std::string &path)
{
  // TODO: read in config file here
  //   use the pre-made PlayerConfig, EnemyConfig, BulletConfig variables

    std::ifstream istr;
    istr.open(path);

    if (!istr.is_open()) 
    {
        std::cerr << "Could not open the file!";
    }

    std::string type;
    while (istr >> type) 
    {
        if (type == "Window") 
        {
            istr >> m_windowConfig.winWidth >> m_windowConfig.winHeight >> m_windowConfig.frameLimit >> m_windowConfig.fullScreen;


        }
        if (type == "Font")
        {
            istr >> m_fontConfig.urlPath >> m_fontConfig.fontSize>> m_fontConfig.colorR>> m_fontConfig.colorG>> m_fontConfig.colorB;


        }
        if (type == "Player")
        {
            istr >> m_playerConfig.SR
                >> m_playerConfig.CR
                >> m_playerConfig.S
                >> m_playerConfig.FR
                >> m_playerConfig.FG
                >> m_playerConfig.FB
                >> m_playerConfig.OR
                >> m_playerConfig.OG
                >> m_playerConfig.OB
                >> m_playerConfig.OT
                >> m_playerConfig.V;


        }
        if (type == "Enemy")
        {
            istr >> m_enemyConfig.SR
                >> m_enemyConfig.CR
                >> m_enemyConfig.SMIN
                >> m_enemyConfig.SMAX
                >> m_enemyConfig.OR
                >> m_enemyConfig.OG
                >> m_enemyConfig.OB
                >> m_enemyConfig.OT
                >> m_enemyConfig.VMIN
                >> m_enemyConfig.VMAX
                >> m_enemyConfig.L
                >> m_enemyConfig.SI;
                


        }
        if (type == "Bullet")
        {
            istr >> m_bulletConfig.SR
                >> m_bulletConfig.CR
                >> m_bulletConfig.S
                >> m_bulletConfig.FR
                >> m_bulletConfig.FG
                >> m_bulletConfig.FB
                >> m_bulletConfig.OR
                >> m_bulletConfig.OG
                >> m_bulletConfig.OB
                >> m_bulletConfig.OT
                >> m_bulletConfig.V
                >> m_bulletConfig.L;


        }

    }

    istr.close();


  // set up default window paramters
  //m_window.create(sf::VideoMode({1280, 720}), "Assignment 2");
  m_window.create(sf::VideoMode({ (uint32_t)m_windowConfig.winWidth, (uint32_t)m_windowConfig.winHeight }), "Assignment 2");
  m_window.setKeyRepeatEnabled(false);
  m_window.setFramerateLimit(m_windowConfig.frameLimit);
  
  

  if(!ImGui::SFML::Init(m_window)) {}

  // scale the imgui ui and text size by 2
  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  spawnPlayer();
}

std::shared_ptr<Entity> Game::player()
{
  return m_entities.getEntities("player").back();
}

void Game::run()
{
  // TODO: add pause functionality in here
  //       some systems should function while paused (rendering)
  //       some systems shouldn't (movement / input)

  while(true)
  {
    // update the entity manager
    m_entities.update();

    // required update call to imgui
    ImGui::SFML::Update(m_window, m_deltaClock.restart());

    sUserInput();
    sEnemySpawner();
    sMovement();
    sCollision();
    sGUI();
    sRender();
  
    // increment the current frame
    // may need to be moved when pause implemented
    m_currentFrame++;
  }
}


void Game::setPaused(bool paused)
{
  // TODO
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
  // TODO: Finish adding all properties of the player with correct values from the configuration file

  // we create every entity by calling EntityManger.addEntity(tag)
  // This returns a shared_ptr<Entity> that we can use to add components
  // so we use 'auto' to save typing
  auto e = m_entities.addEntity("player");


  // Give this entity a Transform so it spawns at (200, 200), with velocity (1,1) and angle 0
  e->add<CTransform>(Vec2(m_windowConfig.winWidth / 2, m_windowConfig.winHeight / 2), Vec2f(0.f, 0.f), 0.0f);

  // The entity;s shape will have a radius 32, 8 sides, dark grey fill, and red outline of thickness 4
  e->add<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);


  // Add an input component to the player so that we can use inputs
  e->add<CInput>();
}


// spawn an enemy at a random position
void Game::spawnEnemy()
{
  // TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
  //      the enemy must be spawned completely within the bounds of the window

  // record when the most recent enemy was spawned
  m_lastEnemeySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
  // TODO: spawn small enemies at the location of the input enemy e

  // when we create the smaller enemy, we have to read the values of the original enemy
  // - spawn a number of small enemies equal to the vertices of the original enemy
  // - set each small enemy to the same color as the original, half the size
  // - small enemies are worth double points of the original enemy
}


// spawns a bullet from a give entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f & target)
{
  // TO-DO: implement the spawning of a bullet which travels towards target
  //       - bullet speed is given as a scalar speed
  //       - you must set the velocity by using formula in notes 
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
  // TO-DO: implement your own special weapon
}

void Game::sMovement()
{
  // TO-DO: implement all entity movement in this function
  //        you should read the m_player->cInput component to determine if the player is moving up or down or left or right
  
    auto& input = player()->get<CInput>();
    auto& transform = player()->get<CTransform>();
  

    if (input.up && input.left)
    {
        std::cout << "X:" << transform.pos.x + m_playerConfig.S << std::endl;

        Vec2f(transform.pos.x + m_playerConfig.S, transform.pos.y + m_playerConfig.S).normalize();
    }

    if (input.up) 
    {
        transform.pos.y -= transform.velocity.y + m_playerConfig.S;
    }
    if (input.left)
    {
        transform.pos.x -= transform.velocity.x + m_playerConfig.S;
        std::cout << "X:" << transform.pos.x + m_playerConfig.S << std::endl;

    }
    if (input.down)
    {
        transform.pos.y += transform.velocity.y + m_playerConfig.S;
    }
    if (input.right)
    {
        transform.pos.x += transform.velocity.x + m_playerConfig.S;
    }

   

  // sample movement update for the player
  
 // transform.pos.x += transform.velocity.x;
  //transform.pos.y += transform.velocity.y;
}
  
void Game::sLifespan()
{
  // TO-DO: implement all lifespan functionality

  // for all entities
  //     if entity has no lifespan component, skip it
  //     if entity has > 0 remanining lifespan, subtract 1
  //     if it has lifespan and is alive 
  //        scale its alpha channel properly
  //     if it has lifespan and its time is up
  //        destroy the entity

}

void Game::sCollision()
{
  // TO-DO: implement all proper collisions between entities
  //        be sure to use the collision radius, NOT the shape radius
  for(auto b: m_entities.getEntities("bullet"))
  {
    for(auto e: m_entities.getEntities("enemy"))
    {
      // do collision logic
    }

    for(auto e: m_entities.getEntities("smallEnemy"))
    {
      // do collision logic 
    }
  }
}

void Game::sGUI()
{
  ImGui::Begin("Geometry Wars");

  ImGui::Text("Stuff Goes Here");

  ImGui::End();
}

void Game::sEnemySpawner()
{
  // TO-DO: implement enemy spawning logic here
  //        use m_enemyConfig to set enemy properties
  //        use m_lastEnemySpawnTime and m_currentFrame to determine when to spawn next enemy
}

void Game::sRender()
{
  if(!m_window.isOpen())
    return;

  // TO-DO: change the code below to draw ALL of the entities
  //        sample drawing of the player Entity that we have created
  m_window.clear();

  // set the position of the shape based on the entity's transform 
  player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);

  // set the rotation of the shape based on the entity's transform->angle
  player()->get<CTransform>().angle += 1.0f;
  player()->get<CShape>().circle.setRotation(sf::degrees(player()->get<CTransform>().angle));

  // draw the entity's sf::CircleShape
  m_window.draw(player()->get<CShape>().circle);

  // draw the ui last
  ImGui::SFML::Render(m_window);

  m_window.display();
}

void Game::sUserInput()
{
  // TO-DO: handle user input here
  //       note that you should only be setting the player's input component variables here

  //       you should not implement the player's movement logic here
  //       the movement system will read the variables you set in this function

  while(auto event = m_window.pollEvent())
  {
      auto& input = player()->get<CInput>();
    // pass the event to imgui to be parsed
    ImGui::SFML::ProcessEvent(m_window, *event);

    // this event triggers when the window is closed
    if(event->is<sf::Event::Closed>())
    {
      std::exit(0);
    }

    // this event is triggered when a key is pressed
    if(const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
      // print the key that was pressed to the console
      std::cout << "Key pressed = " << int(keyPressed->scancode) << "\n";

      if(keyPressed->scancode == sf::Keyboard::Scancode::W)
      {
        // TO-DO: set player's input component UP to true
        std::cout << "W key Pressed\n";
        input.up = true;
      }
      if (keyPressed->scancode == sf::Keyboard::Scancode::A)
      {
          // TO-DO: set player's input component UP to true
          std::cout << "A key Pressed\n";
          input.left = true;
      }
      if (keyPressed->scancode == sf::Keyboard::Scancode::S)
      {
          // TO-DO: set player's input component UP to true
          std::cout << "S key Pressed\n";
          input.down = true;
      }
      if (keyPressed->scancode == sf::Keyboard::Scancode::D)
      {
          // TO-DO: set player's input component UP to true
          std::cout << "D key Pressed\n";
          input.right = true;
      }

    }

    // this event is triggered when a key is released
    if(const auto *keyReleased = event->getIf<sf::Event::KeyReleased>())
    {
      // print the key that was released to the console
      std::cout << "Key released = " << int(keyReleased->scancode) << "\n";

      if(keyReleased->scancode == sf::Keyboard::Scancode::W)
      {
        // TO-DO: set player's input component UP to false
        std::cout << "W key Released\n";
        input.up = false;

      }
      if (keyReleased->scancode == sf::Keyboard::Scancode::A)
      {
          // TO-DO: set player's input component UP to false
          std::cout << "A key Released\n";
          input.left = false;

      }
      if (keyReleased->scancode == sf::Keyboard::Scancode::S)
      {
          // TO-DO: set player's input component UP to false
          std::cout << "S key Released\n";
          input.down = false;
      }
      if (keyReleased->scancode == sf::Keyboard::Scancode::D)
      {
          // TO-DO: set player's input component UP to false
          std::cout << "D key Released\n";
          input.right = false;
      }

    }

    if(const auto *mousePressed  = event->getIf<sf::Event::MouseButtonPressed>())
    {
      Vec2f mpos(mousePressed->position);

      if(mousePressed->button == sf::Mouse::Button::Left)
      {
        // TO-DO: spawn a bullet towards the mouse position
        std::cout << "Left Mouse Button Pressed\n";
        std::cout << "Mouse Position: (" << mpos.x << ", " << mpos.y << ")\n";
      }
      else if (mousePressed->button == sf::Mouse::Button::Right)
      {
        // TO-DO: spawn special weapon
        std::cout << "Right Mouse Button Pressed\n";
        std::cout << "Mouse Position: (" << mpos.x << ", " << mpos.y << ")\n";
      }
    }
  }
}