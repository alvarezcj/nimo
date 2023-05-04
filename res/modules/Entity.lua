Entity = {}
Entity.__index = Entity

function Entity.new()
    return setmetatable({id = ""}, Entity)
end
setmetatable(Entity, { __call = function(_, ...) return Entity.new(...) end })
