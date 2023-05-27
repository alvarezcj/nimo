
nimo.Entity.Coroutine = {}
nimo.Entity.Coroutine.Internal = {}

local nextId
do
  local _NEXTID = 1
  nextId = function()
    local temp = _NEXTID
    _NEXTID = _NEXTID + 1
    return temp
  end
end

local _ROUTINES = {}

nimo.Entity.Coroutine.Internal._ROUTINES = {}
nimo.Entity.Coroutine.Internal.nextId = nextId
nimo.Entity.Coroutine.Internal._NEXTID = _NEXTID

local function ResumeCoroutine(owner, co)
    local ok, delay
    if coroutine.status(co.thread) ~= "dead" then
      ok, delay = coroutine.resume(co.thread, table.unpack(co.params))
      if not ok then
        error(delay, 0)
      end
    end
    if type(delay) == "number" then
      co.nextExec = nimo.Time.time + delay
    end
    if coroutine.status(co.thread) == "dead" then
        nimo.Entity.Coroutine.Internal._ROUTINES[owner][co.id] = nil
    end
end
nimo.Entity.Coroutine.Internal.Resume = ResumeCoroutine

local function StartCoroutine(owner, func, ...)
    if type(func) == "function" then
      func = coroutine.create(func)
    end
    if type(func) == "thread" then
        local id = nimo.Entity.Coroutine.Internal.nextId()
        local params = {...}
        if nimo.Entity.Coroutine.Internal._ROUTINES[owner] == nil then
            nimo.Entity.Coroutine.Internal._ROUTINES[owner] = {}
        end
        nimo.Entity.Coroutine.Internal._ROUTINES[owner][id] = {id=id; params=params; thread=func; nextExec=0.0}
        return id
    end
    return nil
end

local function Run(owner)
    local cos = nimo.Entity.Coroutine.Internal._ROUTINES[owner]
    if cos then
        for k,v in pairs(cos) do
            if nimo.Time.time >= v.nextExec then
                local delay = nimo.Entity.Coroutine.Internal.Resume(owner, v)
            end
        end
    end
end

local function KillCoroutine(owner, id)
  local co = nimo.Entity.Coroutine.Internal._ROUTINES[owner][id]
  if co then
    nimo.Entity.Coroutine.Internal._ROUTINES[owner][co.id] = nil
  end
end
local function KillAllCoroutines(owner)
    local co = nimo.Entity.Coroutine.Internal._ROUTINES[owner]
    if co then
      nimo.Entity.Coroutine.Internal._ROUTINES[owner] = nil
    end
end

nimo.Entity.Coroutine.Start = StartCoroutine
nimo.Entity.Coroutine.Stop = KillCoroutine
nimo.Entity.Coroutine.StopAll = KillAllCoroutines
nimo.Entity.Coroutine.Internal.Run = Run


