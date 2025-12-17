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
      // TODO - implement add(+)
       

      return Vec2(rhs.x + x, rhs.y + y);
    }

    Vec2 operator - (const Vec2& rhs) const 
    {
      // TODO
      return Vec2(rhs.x - x, rhs.y - y);
    }

    Vec2 operator / (const T val) const 
    {
      // TODO - NEED CHECK later
      return Vec2(val.x/x, val.y / y);
    }

    Vec2 operator * (const T val) const 
    {
      // TODO - NEED CHECK later
      return Vec2(val.x / x, val.y / y);
    }

    bool operator == (const Vec2& rhs) const 
    {
      // TODO - DONE

        if (x == rhs.x && y == rhs.y) return true;

      return false;
    }

    bool operator != (const Vec2& rhs) const 
    {
      // TODO
       if (x != rhs.x && y != rhs.y) return true;

      return false;
    }

    void operator += (const Vec2& rhs)
    {
      // TODO - not sure
        x += rhs.x;
        y += rhs.y;
    }

    void operator -= (const Vec2& rhs)
    {
      // TODO -DONE
        x -= rhs.x;
        y -= rhs.y;
    }

    void operator *= (const T val)
    {
      // TODO - DONE
        x *= val.x;
        y *= val.y;
    }

    void operator /= (const T val)
    {
      // TODO - DONE
        x /= val.x;
        y /= val.y;
    }

    float distance(const Vec2& rhs) const 
    {
      // TODO - not sure

         return sqrtf(pow(rhs.x - x) + pow(rhs.y - y));     
    }

    float length() const 
    {
      // TODO - not sure

      return sqrtf(x * x + y * y);
    }

    void normalize()
    {
      // TODO - not sure

        float L = this->length();
        this. x = this.x / L;
        this. y = this.y / L;

    }
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;