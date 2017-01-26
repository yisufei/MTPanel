package.path = package.path..';../conf/?.lua'

require('util')
local config = require('demo_len')

function displayCommon(config)
    local a = config['common']['req']['content'];
    for k, v in pairs(a) do 
        print(k, v['name'], v['length'])
    end
end

function display(t)
    for k, v in pairs(t) do 
        print(k, v)
    end
end

--displayCommon(config)

local P = {}

LFixedLen = P

function P:new(funcid, msg)
    o = {funcid = funcid, msg = msg}
    o.config = config
    setmetatable(o, self)
    self.__index = self
    return o
end

function P:parseReq()
    local common = self.config['common']['req']['content']
    local single = self.config[self.funcid]['req']

    --[=[print('common')
    displayCommon(self.config)
    display(single)
    print(self.msg)]=]

    local i
    local s = 1
    local req = {}
    for k, v in pairs(common) do 
       local l = v['length']
       req[v['name']] = trim(string.sub(self.msg, s, s + l -1))
       s = s + l
    end
    for k, v in pairs(single) do 
       local l = v['length']
       req[v['name']] = trim(string.sub(self.msg, s, s + l -1))
       s = s + l
    end
    return req
end

function P:businessLogic(req)
    req['TranDate'] = '20170124'
    req['TranTime'] = '111404'
    req['TranSeq'] = 'seq001'
    req['RspCod'] = '0000'
    req['RspMsg'] = 'succ'
    return req
end

function P:genStr(format, value, padding)
    if padding == nil then padding = ' ' end
    value = value or ''
    local len = string.len(value)
    local str = string.rep(padding, format['length'] - len)..value
    --print(format['name']..':['..str..']')
    return str
end

function P:generateAnsStr(ans)
    local common = self.config['common']['ans']['content']
    local single = self.config[self.funcid]['ans']

    local ansStr = ''
    for k,v in pairs(common) do
        ansStr = ansStr..self:genStr(v, ans[v['name']])
    end
    if ans['RspCod'] == '0000' then
        for k, v in pairs(single) do
            ansStr = ansStr..self:genStr(v, ans[v['name']])
        end
    end
    --print('ansStr:'..ansStr)
    return self:genStr({length = 8, name = ''}, string.len(ansStr), '0')..ansStr
end

function P:process()
    local req = self:parseReq()
    local ans = self:businessLogic(req)
    local ansStr = self:generateAnsStr(ans)
    return ansStr
end

function addAns(req)
    req['TranDate'] = '20170124'
    req['TranTime'] = '111404'
    req['TranSeq'] = 'seq001'
    req['RspCod'] = '0000'
    req['RspMsg'] = 'succ'
    return req
end

--[=[
local x = P:new('QRY00001', 'txncod1000000120170123231800TermSeq0000000000000000000000001Branch001Teller0001C1QRY00001')
print(x:process())

local req = x:parseReq()
print("req:")
display(req)
local ans = addAns(req)
print("ans")
display(ans)
local ansStr = x:generateAnsStr(ans)
print('['.. ansStr.. ']')]=]

return LFixedLen

