local this = {}
this.id = 8
this.guid = "adfdsaf"
function this:OnCreate()
    nimo.Debug.Log(package.path)
    nimo.Entity.GetComponent(this.entity)
end
function this:OnUpdate(deltaTime)
    nimo.Debug.Log(tostring(deltaTime))
end
function this:OnDestroy()
end
return this