#pragma once 

#include "entt/entt.hpp"

namespace nimo{
    class Entity{
    public:
        Entity(entt::entity handle, entt::registry& registry);
        ~Entity();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_registry.get<T>(m_handle);
		}

		template<typename T>
		const T& GetComponent() const
		{
			return m_registry.get<T>(m_handle);
		}

		template<typename... T>
		bool HasComponent()
		{
			return m_registry.all_of<T...>(m_handle);
		}

		template<typename... T>
		bool HasComponent() const
		{
			return m_registry.all_of<T...>(m_handle);
		}

		template<typename...T>
		bool HasAny()
		{
			return m_registry.any<T...>(m_handle);
		}

		template<typename...T>
		bool HasAny() const
		{
			return m_registry.any<T...>(m_handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_registry.remove<T>(m_handle);
		}

    private:
        entt::entity m_handle = entt::null;
        entt::registry& m_registry;
    };
};