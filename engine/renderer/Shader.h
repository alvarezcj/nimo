#pragma once
#include <string>
#include <glm/glm.hpp>
#include "assets/Asset.h"

namespace nimo{
class Shader : public Asset{
public:
    Shader(const std::string& filename);
    ~Shader();
    static AssetType GetType(){ return AssetType::Shader;}
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

private:
    unsigned int ID;
    void checkCompileErrors(unsigned int shader, const std::string& type);
};
};