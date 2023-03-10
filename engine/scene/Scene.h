#pragma once
#include "Entity.h"
#include "Components.h"
#include "renderer/Shader.h"
#include <string>
#include "assets/Asset.h"
#include <functional>

namespace nimo{
    class Scene : public Asset{
    public:
        Scene(const std::string& name = "NewScene");
        ~Scene();
        static AssetType GetType(){ return AssetType::Scene;}
        void Update(const Shader& shader);
        void ForEachEntity(std::function<void(Entity&)> action);
        Entity CreateEntity(const std::string& name = "");
        Entity CreateEntityWithID(GUID desiredId);
        void DestroyEntity(Entity entity);
        void SetName(const std::string& newName) {name = newName;}
        Entity GetEntity(GUID id) {return {m_entities[id], m_registry};}
    private:
        std::string name; 
        entt::registry m_registry;
        std::unordered_map<GUID, entt::entity> m_entities;

        friend class SceneSerializer;
        friend struct AssetSerializer<Scene>;
    };
};