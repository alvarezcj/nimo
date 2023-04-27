#pragma once
#include "Script.h"
#include <memory>

namespace nimo
{
class ScriptInstance
{
public:
    int stackReference;
    std::shared_ptr<Script> script;
};
} // namespace nimo
