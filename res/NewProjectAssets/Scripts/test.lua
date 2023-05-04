local test = {
    speed = 1.0,
    Public = {
        field1 = 1,
        field2 = "asdasd",
        field3 = true
    }
}

function test:OnCreate()
    nimo.Debug.Log("ASD")
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
    local transform = nimo.Entity.GetComponent(self.entity, "Transform")
    if nimo.Input.GetKey(nimo.KeyCode.I) 
    then
        -- nimo.Entity.Instantiate("prefab1")
    end
    if nimo.Input.GetKey(nimo.KeyCode.K) 
    then
        nimo.Entity.Destroy(self.entity)
    end
    if nimo.Input.GetKey(nimo.KeyCode.D) 
    then
        transform.Rotation.y = transform.Rotation.y + self.Public.field1 * nimo.Time.deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.A) 
    then
        transform.Rotation.y = transform.Rotation.y - self.speed * nimo.Time.deltaTime
    end
    nimo.Entity.SetComponent(self.entity, "Transform", transform)
end

function test:OnDestroy()
end
return test