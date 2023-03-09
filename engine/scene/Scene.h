#pragma once
#include "Entity.h"
#include "Components.h"
#include "renderer/Shader.h"
#include <string>
#include "assets/Asset.h"

namespace nimo{
    class Scene : public Asset{
    public:
        Scene(const std::string& name = "NewScene");
        ~Scene();
        static AssetType GetType(){ return AssetType::Scene;}
        void Update(const Shader& shader);
        bool UpdateGui();
        Entity CreateEntity(const std::string& name = "");
        void DestroyEntity(Entity entity);
        void SetName(const std::string& newName) {name = newName;}
    private:
        std::string name; 
        entt::registry m_registry;

        friend class SceneSerializer;
        friend struct AssetSerializer<Scene>;
    };
};