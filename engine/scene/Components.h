#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "core/GUID.h"
#include "scripting/ScriptInstance.h"

namespace nimo{
struct IDComponent
{
    GUID Id;
};
struct LabelComponent
{
    std::string Label;
};

struct FamilyComponent
{
    GUID Parent;
    std::vector<GUID> Children;

    FamilyComponent(GUID parent) : Parent(parent) {}
    FamilyComponent() = default;
    FamilyComponent(const FamilyComponent& other) = default;
};

struct TransformComponent
{
    glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent& other) = default;
    TransformComponent(const glm::vec3& translation)
        : Translation(translation) {}

    glm::mat4 GetTransform() const
    {
        return glm::translate(glm::mat4(1.0f), Translation)
            * glm::toMat4(glm::quat(Rotation))
            * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct CameraComponent{
    enum class Projection : unsigned int{
        Perspective,
        Orthographic
    } Projection{Projection::Perspective};
    float FOV = 60.0f;
    struct ClippingPlanes{
        float Near{0.1f};
        float Far{100.0f};
    }ClippingPlanes;
};

struct PointLightComponent{
    glm::vec3 Color{1.0f, 1.0f, 1.0f};
    float Intensity;
};

struct MeshComponent{
    std::shared_ptr<Mesh> source;
};

struct MeshRendererComponent{
    std::shared_ptr<Material> material;
};
struct ScriptComponent{
    std::vector<ScriptInstance> instances;
};
};