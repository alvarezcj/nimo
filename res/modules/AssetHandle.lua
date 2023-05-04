local AssetHandle = {}
AssetHandle.__index = AssetHandle

function AssetHandle.new(assetType)
    return setmetatable({id = "", assetType = assetType or 0}, AssetHandle)
end
setmetatable(AssetHandle, { __call = function(_, ...) return AssetHandle.new(...) end })
return AssetHandle
