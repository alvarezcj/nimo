#include "Entity.h"
#include <iostream>

nimo::Entity::Entity(entt::entity handle, entt::registry& registry)
    : m_handle(handle)
    , m_registry(registry)
{

}
nimo::Entity::~Entity()
{
}