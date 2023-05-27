local CameraController = {
    lastMousePositionX = 0,
    lastMousePositionY = 0,
    Public = {
        speed = 1,
        sensivity = 30
    }
}
function CameraController:f()
    nimo.Debug.Log("Print this and wait 1 sec...")
    coroutine.yield(1)
    nimo.Debug.Log("Print this and wait 2 secs...")
    coroutine.yield(2)
    nimo.Debug.Log("Print this and exit")
end

function CameraController:OnCreate()
    nimo.Debug.Log("CameraController OnCreate")
    lastMousePositionX, lastMousePositionY = nimo.Input.GetMousePosition()
    nimo.Entity.Coroutine.Start(self.entity, self.f)
end

function CameraController:OnUpdate()
    if nimo.Input.GetKeyReleased(nimo.KeyCode.Escape) 
    then
        nimo.Application.Close()
    end
    local transform = nimo.Entity.GetComponent(self.entity, "Transform")
    local forward = nimo.Entity.GetForward(self.entity)
    local right = nimo.Entity.GetRight(self.entity)
    local up = nimo.Entity.GetUp(self.entity)
    currentMousePositionX, currentMousePositionY = nimo.Input.GetMousePosition()
    if nimo.Input.GetMouseButton(1) 
    then
        transform.Rotation.y = transform.Rotation.y + self.Public.sensivity * nimo.Time.deltaTime * (currentMousePositionX - lastMousePositionX)
        transform.Rotation.x = transform.Rotation.x + self.Public.sensivity * nimo.Time.deltaTime * (currentMousePositionY - lastMousePositionY)
    end
    if nimo.Input.GetKey(nimo.KeyCode.D) 
    then
        transform.Translation.x = transform.Translation.x + right.x * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.y = transform.Translation.y + right.y * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.z = transform.Translation.z + right.z * self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.A) 
    then
        transform.Translation.x = transform.Translation.x - right.x * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.y = transform.Translation.y - right.y * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.z = transform.Translation.z - right.z * self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.W) 
    then
        transform.Translation.x = transform.Translation.x - forward.x * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.y = transform.Translation.y - forward.y * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.z = transform.Translation.z - forward.z * self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.S) 
    then
        transform.Translation.x = transform.Translation.x + forward.x * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.y = transform.Translation.y + forward.y * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.z = transform.Translation.z + forward.z * self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.Space) 
    then
        transform.Translation.x = transform.Translation.x + up.x * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.y = transform.Translation.y + up.y * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.z = transform.Translation.z + up.z * self.Public.speed * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.LeftControl) 
    then
        transform.Translation.x = transform.Translation.x - up.x * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.y = transform.Translation.y - up.y * self.Public.speed * nimo.Time.deltaTime
        transform.Translation.z = transform.Translation.z - up.z * self.Public.speed * nimo.Time.deltaTime
    end
    nimo.Entity.SetComponent(self.entity, "Transform", transform)
    lastMousePositionX = currentMousePositionX;
    lastMousePositionY = currentMousePositionY;
end

function CameraController:OnDestroy()
end
return CameraController