#include "Entity.h"
#include <iostream>
#include "Components.h"

nimo::Entity::Entity(entt::entity handle, entt::registry& registry)
    : m_handle(handle)
    , m_registry(registry)
{

}
nimo::Entity::~Entity()
{
}

nimo::GUID nimo::Entity::ID() const
{
    return GetComponent<IDComponent>().Id;
}