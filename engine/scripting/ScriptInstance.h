#pragma once
#include "Script.h"
#include "core/GUID.h"
#include <memory>

namespace nimo
{
class ScriptInstance
{
public:
    GUID owner;
    int stackReference;
    std::shared_ptr<Script> script;
};
} // namespace nimo
