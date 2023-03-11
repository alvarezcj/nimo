#pragma once
#include "Shader.h"
#include "Texture.h"

namespace nimo
{
    enum class MaterialPropertyType : unsigned int{
        Texture,
        Vector2
    };
    class IMaterialProperty{
    public:
        std::string name;
        MaterialPropertyType type;
        virtual void Setup(Shader* shader){}
        virtual void* GetDataPtr() = 0;
    };
    template<typename T>
    class MaterialProperty : public IMaterialProperty{
    public:
        void Setup(Shader* shader) override {
            shader->set(name, value);
        }
        void* GetDataPtr() override {
            return (void*)&value;
        }
        void SetValue(const T& v){
            value = v;
        }
    private:
        T value;
    };
    class MaterialPropertyTexture : public IMaterialProperty{
    public:
        MaterialPropertyTexture() {type = MaterialPropertyType::Texture;}
        void Setup(Shader* shader) override {
            shader->set(name, value);
            t->bind(value);
        }
        void* GetDataPtr() override {
            return (void*)&value;
        }
        void SetValue(int v){
            value = v;
        }
        int GetValue(){return value;}
        void SetTexture(std::shared_ptr<Texture> v){
            t = v;
        }
        std::shared_ptr<Texture> GetTexture(){return t;}
    private:
        int value;
        std::shared_ptr<Texture> t;
    };
} // namespace nimo