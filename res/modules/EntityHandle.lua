local EntityHandle = {}
EntityHandle.__index = EntityHandle

function EntityHandle.new()
    return setmetatable({entity = ""}, EntityHandle)
end
setmetatable(EntityHandle, { __call = function(_, ...) return EntityHandle.new(...) end })
return EntityHandle
