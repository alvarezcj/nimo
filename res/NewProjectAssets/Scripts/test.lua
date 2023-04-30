local this = {}
this.speed = 1.0
function this:OnCreate()
    nimo.Debug.Log("ASD")
end
function this:OnUpdate(deltaTime)
    local transform = nimo.Entity.GetComponent(this.entity, "Transform")
    if nimo.Input.GetKey(nimo.KeyCode.D) then
    transform.Rotation.y = transform.Rotation.y + this.speed * deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.A) then
    transform.Rotation.y = transform.Rotation.y - this.speed * deltaTime
    end
    nimo.Entity.SetComponent(this.entity, "Transform", transform)
end
function this:OnDestroy()
end
return this