local CameraController = {
    lastMousePositionX = 0,
    lastMousePositionY = 0,
    Public = {
        speed = 1,
        sensivity = 30
    }
}

function CameraController:OnCreate()
    nimo.Debug.Log("CameraController OnCreate")
    lastMousePositionX, lastMousePositionY = nimo.Input.GetMousePosition()
end

function CameraController:OnUpdate()
    if nimo.Input.GetKeyReleased(nimo.KeyCode.Escape) 
    then
        nimo.Application.Close()
    end
    local transform = nimo.Entity.GetComponent(self.entity, "Transform")
    currentMousePositionX, currentMousePositionY = nimo.Input.GetMousePosition()
    if nimo.Input.GetMouseButton(1) 
    then
        transform.Rotation.y = transform.Rotation.y + self.Public.sensivity * nimo.Time.deltaTime * (currentMousePositionX - lastMousePositionX)
        transform.Rotation.x = transform.Rotation.x + self.Public.sensivity * nimo.Time.deltaTime * (currentMousePositionY - lastMousePositionY)
    end
    if nimo.Input.GetKey(nimo.KeyCode.D) 
    then
        transform.Translation.x = transform.Translation.x + self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.A) 
    then
        transform.Translation.x = transform.Translation.x - self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.W) 
    then
        transform.Translation.z = transform.Translation.z + self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.S) 
    then
        transform.Translation.z = transform.Translation.z - self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.Space) 
    then
        transform.Translation.y = transform.Translation.y + self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.LeftControl) 
    then
        transform.Translation.y = transform.Translation.y - self.Public.speed * nimo.Time.deltaTime
    end
    nimo.Entity.SetComponent(self.entity, "Transform", transform)
    lastMousePositionX = currentMousePositionX;
    lastMousePositionY = currentMousePositionY;
end

function CameraController:OnDestroy()
end
return CameraController