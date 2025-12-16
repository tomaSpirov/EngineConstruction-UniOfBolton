#pragma once

#include "Components.hpp"
#include <tuple>

#include <memory>

class EntityManager;

using ComponentTuple = std::tuple<
  CTransform,
  CShape,
  CCollision,
  CInput,
  CScore,
  CLifespan
>;

class Entity
{
  friend class EntityManager;

  ComponentTuple m_components;

  bool m_alive = true; // if false, will delete Entity

  std::string m_tag = "default"; // type of entity

  size_t m_id = 0; // unique id for entity

  public:
    Entity() = default;

    Entity(size_t id, const std::string &tag)
      : m_id(id), m_tag(tag)
	{
	}

    // Member function template
    template<typename T, typename... Args>
    T& add(Args&&... args)
    {
      auto &component = get<T>();
      component = T(std::forward<Args>(args)...);
      component.exists = true;
      return component;
    }

    // Get a component of type T
    template<typename T>
    T &get()
    {
      return std::get<T>(m_components);
    }

    template<typename T>
    const T & get() const
    {
      return std::get<T>(m_components);
    }

    template<typename T>
    bool has() const
    {
      return get<T>().exists;
    }

    template<typename T>
    void remove()
    {
      get<T>() = T();
    }



    size_t id() const { return m_id;  }
    bool isAlive() const { return m_alive; }
	void destroy() { m_alive = false; }
	const std::string &tag() const { return m_tag; }
};

/*
  Usage Example:

  Entity entity;

  entity.add<CTransform>(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 1.0f), 0.0f);
  entity.add<CShape>(raduis);

  CTransform &t = entity.get<CTransform>();
  CShape &s = entity.get<CShape>();



*/