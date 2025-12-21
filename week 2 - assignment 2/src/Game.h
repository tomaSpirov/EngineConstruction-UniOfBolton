#pragma once
#include "Entity.hpp"
#include "EntityManager.hpp"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

#include <string>

struct PlayerConfig 
{ 
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  //  32  32  5   5   5   255 0   0   4  8
  float S;//5
};

struct EnemyConfig 
{
  int SR, CR, FR, FG, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};

struct BulletConfig 
{ 
  int SR, CR, FR, FG,FB, OR, OG, OB, OT, V, L; 
  float S;
};

struct WindowConfig 
{
	float winWidth, winHeight;
	int frameLimit;
	bool fullScreen;
};

struct FontConfig 
{
		std::string urlPath;
		int fontSize;
		int colorR, colorG, colorB;
};




class Game
{
	




  sf::RenderWindow m_window; // the window we will draw to
  EntityManager m_entities; // vector of entities to maintain
  sf::Font m_font; // the font we will use to draw
  sf::Text m_text; // the score text to be drawn to the screen
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  WindowConfig m_windowConfig;
  FontConfig m_fontConfig;

  sf::Clock m_deltaClock;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemeySpawnTime = 0; // time since last enemy spawn
  bool m_paused = false; // whether we update game logic

  void init(const std::string &configFile); // initialize the GameState with a config file
  void setPaused(bool paused); // pause the game

  void sMovement(); // System: Entity position / movement update 
  void sUserInput(); // System: User Input
  void sLifespan(); // System: Lifespan
  void sRender(); // System: Renders / Drawing
  void sGUI(); 
  void sEnemySpawner(); // System: Spawns Enemies
  void sCollision(); // System: Collisions

  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

  std::shared_ptr<Entity> player();

public:
  Game(const std::string &configFile);
  void run();
};