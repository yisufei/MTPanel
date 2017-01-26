
local P = {msg = ""}

LMessageBase = P

function P:new( o , headLen)
	o = o or {msg = ""}
    if headLen == nil or headLen < 1 then
        o['headLen'] = 8        --default
    end
	setmetatable(o, self)
	self.__index = self
	return o
end

function P:getFuncid()
	if self.msg == "" then return "" end
	return string.sub(self.msg, self.headLen + 1, self.headLen + 8)
end

function P:getMsg()
	if self.msg == "" then return "" end
	return string.sub(self.msg, self.headLen + 1)
end

function P:setMsg(msg)
    self.msg = msg
end

--[==[local a = LMessageBase:new({msg = "0123456789abcd"})
print(a.msg)
print(a:getFuncid())
print(a:getMsgContent())

local b = LMessageBase:new()
print(b.msg)  ]==]

return LMessageBase
