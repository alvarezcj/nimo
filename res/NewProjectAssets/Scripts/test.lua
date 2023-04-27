local this = {}
this.id = 8
this.guid = "adfdsaf"
function this:OnCreate()
    nimo.Debug.Log(package.path)
end
function this:OnUpdate()
    nimo.Debug.Log("Update")
end
function this:OnDestroy()
end
return this