#pragma once
#include <string>

namespace nimo
{
enum class ScriptFieldType{
    Number,
    String,
    Boolean
};
struct ScriptField
{
    ScriptField(const std::string name) : name(name){}
    virtual ~ScriptField(){}
    virtual ScriptFieldType GetType() = 0;
    std::string name;
};
struct ScriptFieldNumber : public ScriptField
{
    ScriptFieldNumber(const std::string& name, double value) 
        : ScriptField(name), value(value) {}
    ScriptFieldType GetType() { return ScriptFieldType::Number;};
    float value;
};
struct ScriptFieldString : public ScriptField
{
    ScriptFieldString(const std::string& name, const std::string& value) 
        : ScriptField(name), value(value) {}
    ScriptFieldType GetType() { return ScriptFieldType::String;};
    std::string value;
};
struct ScriptFieldBool : public ScriptField
{
    ScriptFieldBool(const std::string& name, bool value) 
        : ScriptField(name), value(value) {}
    ScriptFieldType GetType() { return ScriptFieldType::Boolean;};
    bool value;
};
} // namespace nimo
