#include "Scene.h"
#include "Components.h"

#include "imgui.h"

#include <iostream>

nimo::Scene::Scene(const std::string& name)
    : name(name)
{

}
nimo::Scene::~Scene()
{

}
void nimo::Scene::Update(const Shader& shader)
{
    auto view1 = m_registry.view<CameraComponent, TransformComponent>();
    view1.each([&](CameraComponent& cam, TransformComponent& t) {
        glm::mat4 projection;
        if(cam.Projection == CameraComponent::Projection::Perspective)
            projection = glm::perspective(glm::radians(cam.FOV), (float)1920 / (float)1080, cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
        else
            projection = glm::ortho(-10.0f * 0.5f, 10.0f * 0.5f, -10.0f * 0.5f *9.0f/16.0f, 10.0f * 0.5f*9.0f/16.0f, 0.1f, 100.0f);
        
        shader.set("view", glm::toMat4(glm::quat(t.Rotation)) * glm::translate(glm::mat4(1.0f), {-t.Translation.x, -t.Translation.y, t.Translation.z}) );
        shader.set("projection", projection);
    });
    auto view3 = m_registry.view<MeshComponent, TransformComponent, MeshRendererComponent>();
    view3.each([&](MeshComponent& m, TransformComponent& t, MeshRendererComponent& r) {
        shader.set("transform", t.GetTransform());
        r.material->Setup();
        r.mesh->draw();
    });
}
bool nimo::Scene::UpdateGui()
{
    float totalHeight = 0;
    ImVec2 rectMin;
    ImVec2 rectMax;
    bool res;
    if(ImGui::CollapsingHeader((name + "##" + id.str()).c_str()))
    {
        rectMin = ImGui::GetItemRectMin();
        rectMax = ImGui::GetItemRectMax();
        totalHeight += ImGui::GetItemRectSize().y;
        m_registry.each([&](entt::entity e)
        {
            Entity ent(e, m_registry);
            std::string entityName =  ent.GetComponent<LabelComponent>().Label;
            if (ImGui::TreeNode((entityName + "##" + id.str()).c_str()))
            {
        totalHeight += ImGui::GetItemRectSize().y;
                if(ent.HasComponent<TransformComponent>())
                {
                    if (ImGui::TreeNode((std::string("Transform##")+entityName).c_str()))
                    {
        totalHeight += ImGui::GetItemRectSize().y;
                        std::string p = "Position##";
                        p += std::to_string((uint32_t)e);
                        std::string r = "Rotation##";
                        r += std::to_string((uint32_t)e);
                        std::string s = "Scale##";
                        s += std::to_string((uint32_t)e);
                        ImGui::DragFloat3(p.c_str(), (float*)&ent.GetComponent<TransformComponent>().Translation, 0.1f);
        totalHeight += ImGui::GetItemRectSize().y;
                        ImGui::DragFloat3(r.c_str(), (float*)&ent.GetComponent<TransformComponent>().Rotation, 0.1f);
        totalHeight += ImGui::GetItemRectSize().y;
                        ImGui::DragFloat3(s.c_str(), (float*)&ent.GetComponent<TransformComponent>().Scale, 0.1f);
        totalHeight += ImGui::GetItemRectSize().y;
                        ImGui::TreePop();
                    }
                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }
                }
                if(ent.HasComponent<CameraComponent>())
                {
                    if (ImGui::TreeNode((std::string("Camera##")+entityName).c_str()))
                    {
        totalHeight += ImGui::GetItemRectSize().y;
                        static int item_current_2 = 0;
                        ImGui::Combo((std::string("Projection##") + (std::string("Camera##")+entityName)).c_str(), &item_current_2, "Perspective\0Orthographic\0\0");
        totalHeight += ImGui::GetItemRectSize().y;
                        if(item_current_2 == 0)
                        {
                            ent.GetComponent<CameraComponent>().Projection = CameraComponent::Projection::Perspective;
                            ImGui::SliderFloat((std::string("FOV##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<CameraComponent>().FOV, 0.1f, 179.0f);
        totalHeight += ImGui::GetItemRectSize().y;
                        }
                        if(item_current_2 == 1)
                        {
                            ent.GetComponent<CameraComponent>().Projection = CameraComponent::Projection::Orthographic;
                        }
                        ImGui::DragFloat((std::string("Clipping plane Far##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<CameraComponent>().ClippingPlanes.Far, 0.1f, 0.0f);
        totalHeight += ImGui::GetItemRectSize().y;
                        ImGui::DragFloat((std::string("Clipping plane Near##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<CameraComponent>().ClippingPlanes.Near, 0.1f, 0.0f);
        totalHeight += ImGui::GetItemRectSize().y;

                        ImGui::TreePop();
                    }
                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }
                }
                if(ent.HasComponent<MeshRendererComponent>())
                {
                    if (ImGui::TreeNode((std::string("Mesh Renderer##")+entityName).c_str()))
                    {
        totalHeight += ImGui::GetItemRectSize().y;
                        if (ImGui::TreeNode((std::string("Material##") + (std::string("Mesh##")+entityName)).c_str()))
                        {
        totalHeight += ImGui::GetItemRectSize().y;
                            auto mat = ent.GetComponent<MeshRendererComponent>().material;
                            for(auto p: mat->properties)
                            {
                                switch (p->type)
                                {
                                case MaterialPropertyType::Vector2:
                                    {
                                        ImGui::DragFloat2((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityName))).c_str(), (float*)p->GetDataPtr(), 0.1f);
        totalHeight += ImGui::GetItemRectSize().y;
                                    }
                                    break;
                                case MaterialPropertyType::Texture:
                                    {
                                        ImGui::InputInt((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityName))).c_str(), (int*)p->GetDataPtr());
        totalHeight += ImGui::GetItemRectSize().y;
                                    }
                                    break;
                                
                                default:
                                    break;
                                }
                            }
                            ImGui::TreePop();
                        }

                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }
                        ImGui::TreePop();
                    }
                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }
                }
                if(ent.HasComponent<MeshComponent>())
                {
                    if (ImGui::TreeNode((std::string("Mesh##")+entityName).c_str()))
                    {
        totalHeight += ImGui::GetItemRectSize().y;
                        ImGui::Text((std::string("Mesh asset:").c_str()));
                        ImGui::SameLine();
                        ImGui::Text((std::string(AssetManager::GetMetadata(ent.GetComponent<MeshComponent>().source->id).filepath.string())).c_str());
        totalHeight += ImGui::GetItemRectSize().y;

                        ImGui::TreePop();
                    }
                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }
                }
                ImGui::TreePop();
            }
                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }
        });
    }
    else{
        totalHeight += ImGui::GetItemRectSize().y;
    }
    res = ImGui::IsMouseHoveringRect(rectMin, ImVec2(rectMax.x, rectMax.y + totalHeight)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    ImGui::Separator();
    return res;
}
nimo::Entity nimo::Scene::CreateEntity(const std::string& name)
{
    auto id = m_registry.create();
    auto e = Entity(id, m_registry);
    e.AddComponent<IDComponent>().Id = GUID::Create();
    if(name == "")
        e.AddComponent<LabelComponent>().Label = e.GetComponent<IDComponent>().Id.str();
    else
        e.AddComponent<LabelComponent>().Label = name;
    return e;
}
void nimo::Scene::DestroyEntity(Entity entity)
{
}