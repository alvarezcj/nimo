local AssetHandle = require("AssetHandle")
local EntityHandle = require("EntityHandle")
local test = {
    speed = 1.0,
    Public = {
        field1 = 1,
        field2 = "asdasd",
        field3 = true,
        prefab = AssetHandle(nimo.AssetType.Prefab),
        otherEntity = EntityHandle()
    }
}

function test:OnCreate()
    nimo.Debug.Log(self.Public.prefab.id)
end

function test:OnUpdate()
    if nimo.Input.GetKey(nimo.KeyCode.Escape) 
    then
        nimo.Application.Close()
    end
    if nimo.Input.GetKey(nimo.KeyCode.M) 
    then
        nimo.Window.Maximize()
    end
    if nimo.Input.GetKey(nimo.KeyCode.N) 
    then
        nimo.Window.Restore()
    end
    if nimo.Input.GetKey(nimo.KeyCode.L) 
    then
        nimo.SceneManagement.LoadScene("Second")
    end
    local transform = nimo.Entity.GetComponent(self.Public.otherEntity, "Transform")
    if nimo.Input.GetKeyReleased(nimo.KeyCode.I) 
    then
        nimo.Entity.Instantiate(self.Public.prefab)
    end
    if nimo.Input.GetKey(nimo.KeyCode.K) 
    then
        nimo.Entity.Destroy(self.entity)
    end
    if nimo.Input.GetKey(nimo.KeyCode.D) 
    then
        transform.Rotation.y = transform.Rotation.y + 30.0 * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.A) 
    then
        transform.Rotation.y = transform.Rotation.y - 30.0 * nimo.Time.deltaTime
    end
    nimo.Entity.SetComponent(self.Public.otherEntity, "Transform", transform)
end

function test:OnDestroy()
end
return test