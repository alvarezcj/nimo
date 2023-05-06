local ContinousRotation = {
    Public = {
        speed = 30
    }
}

function ContinousRotation:OnCreate()
end

function ContinousRotation:OnUpdate()
    local transform = nimo.Entity.GetComponent(self.entity, "Transform")
    transform.Rotation.y = transform.Rotation.y + self.Public.speed * nimo.Time.deltaTime
    nimo.Entity.SetComponent(self.entity, "Transform", transform)
end

function ContinousRotation:OnDestroy()
end
return ContinousRotation