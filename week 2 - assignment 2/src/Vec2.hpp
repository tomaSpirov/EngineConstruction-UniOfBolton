#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>

template<typename T>
class Vec2
{
  public:
    T x = 0;
    T y = 0;

    Vec2() = default;

    Vec2(T xin, T yin)
      : x(xin), y(yin)
    { }

    // constructor to conver from sf::Vector2
    Vec2(const sf::Vector2<T>& v)
      : x(v.x), y(v.y)
    { }

    Vec2(const sf::Vector2i& v)
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
    { }

    // allow automatic convertion to sf::Vector2
    // this lets us pass Vec2 into sfml functions
    operator sf::Vector2<T>()
    {
      return sf::Vector2<T>(x, y);
      // usage example:
      // sf::Sprite sprite;
      // sprite.setPosition(Vec2<float>(10.0f, 20.0f));
    }

    Vec2 operator + (const Vec2& rhs) const 
    {
      // TODO
      return Vec2();
    }

    Vec2 operator - (const Vec2& rhs) const 
    {
      // TODO
      return Vec2();
    }

    Vec2 operator / (const T val) const 
    {
      // TODO
      return Vec2();
    }

    Vec2 operator * (const T val) const 
    {
      // TODO
      return Vec2();
    }

    bool operator == (const Vec2& rhs) const 
    {
      // TODO
      return false;
    }

    bool operator != (const Vec2& rhs) const 
    {
      // TODO
      return false;
    }

    void operator += (const Vec2& rhs)
    {
      // TODO
    }

    void operator -= (const Vec2& rhs)
    {
      // TODO
    }

    void operator *= (const T val)
    {
      // TODO
    }

    void operator /= (const T val)
    {
      // TODO
    }

    float distance(const Vec2& rhs) const 
    {
      // TODO
      return 0.0f;
    }

    float length() const 
    {
      // TODO
      return 0.0f;
    }

    void normalize()
    {
      // TODO
    }
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;