local this = {}
this.speed = 1.0
function this:OnCreate()
    nimo.Debug.Log("ASD")
end
function this:OnUpdate(deltaTime)
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
    local transform = nimo.Entity.GetComponent(this.entity, "Transform")
    if nimo.Input.GetKey(nimo.KeyCode.I) 
    then
        -- nimo.Entity.Instantiate("prefab1")
    end
    if nimo.Input.GetKey(nimo.KeyCode.K) 
    then
        nimo.Entity.Destroy(this.entity)
    end
    if nimo.Input.GetKey(nimo.KeyCode.D) 
    then
        transform.Rotation.y = transform.Rotation.y + this.speed * deltaTime
    end
    if nimo.Input.GetKey(nimo.KeyCode.A) 
    then
        transform.Rotation.y = transform.Rotation.y - this.speed * deltaTime
    end
    nimo.Entity.SetComponent(this.entity, "Transform", transform)
end
function this:OnDestroy()
end
return this