#pragma once
#include "Entity.h"
#include "Components.h"
#include <string>
#include "assets/Asset.h"
#include <functional>
#include "renderer/FrameBuffer.h"

namespace nimo{
    class Scene : public Asset{
    public:
        Scene(const std::string& name = "NewScene");
        ~Scene();
        AssetType Type() const { return AssetType::Scene; }
        static AssetType StaticType(){return AssetType::Scene;}
        void Update(std::shared_ptr<FrameBuffer> target);
        void ForEachEntity(std::function<void(Entity&)> action);
        Entity CreateEntity(const std::string& name = "");
        Entity CreateEntityWithID(GUID desiredId);
        Entity CreateEntityWithParent(Entity parent, const std::string& name = "");
        void DestroyEntity(Entity entity);
        void SetName(const std::string& newName) {name = newName;}
        const std::string& GetName() {return name;}
        Entity GetEntity(GUID id) {return {m_entities[id], m_registry};}
        glm::mat4 GetWorldSpaceTransformMatrix(Entity entity);
    private:
        std::string name; 
        entt::registry m_registry;
        std::unordered_map<GUID, entt::entity> m_entities;
        std::shared_ptr<FrameBuffer> gBuffer;

        friend class SceneSerializer;
        friend struct AssetSerializer<Scene>;
    };
};