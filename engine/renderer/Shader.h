#pragma once
#include <string>
#include <glm/glm.hpp>
#include "assets/Asset.h"

namespace nimo{
enum class ShaderUniformDataType : unsigned int{
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool,
    Mat2,
    Mat3,
    Mat4,
    Sampler1D,
    Sampler2D,
    Sampler3D,
    SamplerCube,
};
struct ShaderUniform{
    std::string name;
    ShaderUniformDataType type;
};
class Shader : public Asset{
public:
    Shader(const std::string& filename);
    ~Shader();
    AssetType Type() const { return AssetType::Shader; }
    static AssetType StaticType(){return AssetType::Shader;}
    void use();
    void set(const std::string &name, bool value) const;
    void set(const std::string &name, int value) const;
    void set(const std::string &name, float value) const;
    void set(const std::string &name, const glm::vec2 &value) const;
    void set(const std::string &name, float x, float y) const;
    void set(const std::string &name, const glm::vec3 &value) const;
    void set(const std::string &name, float x, float y, float z) const;
    void set(const std::string &name, const glm::vec4 &value) const;
    void set(const std::string &name, float x, float y, float z, float w) const;
    void set(const std::string &name, const glm::mat2 &mat) const;
    void set(const std::string &name, const glm::mat3 &mat) const;
    void set(const std::string &name, const glm::mat4 &mat) const;
    
    inline const std::vector<ShaderUniform>& GetUniforms() { return m_uniforms;}

private:
    unsigned int ID;
    void checkCompileErrors(unsigned int shader, const std::string& type);
    std::vector<ShaderUniform> m_uniforms;
};
};