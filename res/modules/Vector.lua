Vector2 = {}
Vector2.__index = Vector2

function Vector2.__add(a, b)
  if type(a) == "number" then
    return Vector2.new(b.x + a, b.y + a)
  elseif type(b) == "number" then
    return Vector2.new(a.x + b, a.y + b)
  else
    return Vector2.new(a.x + b.x, a.y + b.y)
  end
end

function Vector2.__sub(a, b)
  if type(a) == "number" then
    return Vector2.new(b.x - a, b.y - a)
  elseif type(b) == "number" then
    return Vector2.new(a.x - b, a.y - b)
  else
    return Vector2.new(a.x - b.x, a.y - b.y)
  end
end

function Vector2.__mul(a, b)
  if type(a) == "number" then
    return Vector2.new(b.x * a, b.y * a)
  elseif type(b) == "number" then
    return Vector2.new(a.x * b, a.y * b)
  else
    return Vector2.new(a.x * b.x, a.y * b.y)
  end
end

function Vector2.__div(a, b)
  if type(a) == "number" then
    return Vector2.new(b.x / a, b.y / a)
  elseif type(b) == "number" then
    return Vector2.new(a.x / b, a.y / b)
  else
    return Vector2.new(a.x / b.x, a.y / b.y)
  end
end

function Vector2.__eq(a, b)
  return a.x == b.x and a.y == b.y
end

function Vector2.__lt(a, b)
  return a.x < b.x or (a.x == b.x and a.y < b.y)
end

function Vector2.__le(a, b)
  return a.x <= b.x and a.y <= b.y
end

function Vector2.__tostring(a)
  return "(" .. a.x .. ", " .. a.y .. ")"
end

function Vector2.new(x, y)
  return setmetatable({ x = x or 0, y = y or 0 }, Vector2)
end

function Vector2.distance(a, b)
  return (b - a):len()
end

function Vector2:clone()
  return Vector2.new(self.x, self.y)
end

function Vector2:unpack()
  return self.x, self.y
end

function Vector2:len()
  return math.sqrt(self.x * self.x + self.y * self.y)
end

function Vector2:lenSq()
  return self.x * self.x + self.y * self.y
end

function Vector2:normalize()
  local len = self:len()
  self.x = self.x / len
  self.y = self.y / len
  return self
end

function Vector2:normalized()
  return self / self:len()
end

function Vector2:rotate(phi)
  local c = math.cos(phi)
  local s = math.sin(phi)
  self.x = c * self.x - s * self.y
  self.y = s * self.x + c * self.y
  return self
end

function Vector2:rotated(phi)
  return self:clone():rotate(phi)
end

function Vector2:perpendicular()
  return Vector2.new(-self.y, self.x)
end

function Vector2:projectOn(other)
  return (self * other) * other / other:lenSq()
end

function Vector2:cross(other)
  return self.x * other.y - self.y * other.x
end

setmetatable(Vector2, { __call = function(_, ...) return Vector2.new(...) end })