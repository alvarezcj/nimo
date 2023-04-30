Entity = {}
Entity.__index = Entity

function Entity.new()
    return setmetatable({id = id or ""}, Entity)
end
function Entity:GetComponent(strType)
    return nimo.GetEntityComponent(self.id, strType)
end
setmetatable(Entity, { __call = function(_, ...) return Entity.new(...) end })
