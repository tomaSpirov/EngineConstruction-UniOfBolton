#pragma once

#include "Vec2.hpp"

class Component
{
  public:
    bool exists = false;
};

class CTransform : public Component
{
  public:
    Vec2f pos = {0.0, 0.0};
    Vec2f velocity = {0.0, 0.0};
    float angle = 0;

    CTransform() = default;
    CTransform(const Vec2f& position, const Vec2f& vel, float ang)
      : pos(position), velocity(vel), angle(ang)
    {
    }
};

class CShape : public Component
{
public:
  sf::CircleShape circle;

  CShape() = default;
  CShape(float radius, size_t points, const sf::Color &fill, const sf::Color &outline, float outlineThickness)
    : circle(radius, points)
  {
    circle.setFillColor(fill);
    circle.setOutlineColor(outline);
    circle.setOutlineThickness(outlineThickness);
    circle.setOrigin({radius, radius}); // set origin to center
  }
};

class CCollision: public Component
{
  public:
    float radius = 0;

    CCollision() = default;
    CCollision(float r)
      : radius(r)
    {
    }
};

class CScore : public Component
{
  public:
    int score = 0;

    CScore() = default;
    CScore(int s)
      : score(s)
    {
    }
};


class CLifespan : public Component
{
  public:
    int lifespan = 0; // in seconds
    int remainingLifespan = 0;
    CLifespan() = default;
    CLifespan(int totalLifespan)
      : lifespan(totalLifespan), remainingLifespan(totalLifespan) { }
};



class CInput : public Component
{
  public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;

    CInput() = default;
};