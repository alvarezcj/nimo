local this = {}
this.speed = 1.0
function this:OnCreate()
    nimo.Debug.Log("ASD")
end
function this:OnUpdate(deltaTime)
    local transform = nimo.Entity.GetComponent(this.entity, "Transform")
    transform.Rotation.y = transform.Rotation.y + this.speed * deltaTime
    nimo.Entity.SetComponent(this.entity, "Transform", transform)
end
function this:OnDestroy()
end
return this