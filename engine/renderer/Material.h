#pragma once

#include "assets/Asset.h"
#include "json.hpp"
#include <fstream>
#include "assets/AssetManager.h"
#include "Shader.h"
#include "MaterialProperties.h"
#include "core/Log.h"

namespace nimo{
 
    class Material : public Asset{
    public:
        Material(const std::shared_ptr<Shader>& shader, std::vector<IMaterialProperty*> properties)
            : shader(shader)
            , properties(properties)
        {}
        Material(const std::string& filepath)
        {
            NIMO_DEBUG("nimo::Material::Material({})", filepath);
            std::ifstream fin(filepath);
            nlohmann::json j;
            j << fin;
            shader = AssetManager::Get<Shader>(AssetId(std::string(j["shader"])));
            for(auto o : j["properties"])
            {
                if(o["type"] == "vector2")
                {
                    auto p = new MaterialProperty<glm::vec2>();
                    p->name = o["name"];
                    p->type = MaterialPropertyType::Vector2;
                    p->SetValue({o["value"][0], o["value"][1]});
                    properties.push_back(p);
                }
                if(o["type"] == "texture")
                {
                    auto p = new MaterialPropertyTexture();
                    p->name = o["name"];
                    p->type = MaterialPropertyType::Texture;
                    p->SetValue(o["value"]);
                    p->SetTexture(AssetManager::Get<Texture>(AssetId(std::string(o["asset"]))));
                    properties.push_back(p);
                }
            }
        }
        virtual ~Material(){
            NIMO_DEBUG("nimo::Material::~Material");
            for(auto p : properties)
                delete p;
        }
        static AssetType GetType(){return AssetType::Material;}
        void Setup(){
            for(auto p : properties)
                p->Setup(shader.get());
        }
        std::shared_ptr<Shader> shader;
        std::vector<IMaterialProperty*> properties;
    };
};