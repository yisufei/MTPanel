
local P = {}
MTSql = P

local json = require('json')
local sqlite3 = require('lsqlite3')

function P:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function P:open(filename)
    self.file = filename
    self.db = sqlite3.open(self.file)
    if self.db ~= nil then 
        return true 
    else 
        return false 
    end
end

function P:exec(sql)
    return self.db:exec(sql)
end

function P:select(sql)
    local i = 1
    local t = {}
    --print('['..sql..']')
    for row in self.db:nrows(sql) do
        t[i] = row
        i = i+1
    end
    return t, json.encode(t)
end

function P:close()
    self.db:close()
end

--[=[local x = P:new()
x:open('test.db')
print(x:select('select * from t'))
]=]

return MTSql
