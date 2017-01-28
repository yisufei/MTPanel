
require('util')
local LMessageBase = require('LMessageBase')
local LFixedLen = require('LFixedLen')
--local MTSql = require('MTSql')
local MTSql = {}

local P = {msgOp = nil}

QRY00001 = P

function P:new(o)
    if o['msgOp'] == nil then o = {msgOp = nil} end
    setmetatable(o, self)
    self.__index = self
    return o
end

function P:setMsgOp(msgOp)
    self.msgOp = msgOp
end

function P:businessLogic(req)
    req['TranDate'] = '20170124'
    req['TranTime'] = '111404'
    req['TranSeq'] = 'seq001'
    req['RspCod'] = '0000'
    req['RspMsg'] = 'succ'
    
    --[=[local dbOp = MTSql:new()
    dbOp:open('./db/demo.db')
    local t, str = dbOp:select("select * from user where BDOAC221 = '"..req['BDRAC22'].."'")
    dbOp:close()
    
    --displayTable(t)
    --print(str)
    extend(req, t[1])
    --displayTable(req)]=]
    return req
end

function P:process()
    local msg = self.msgOp:getMsg()
    local funcid = self.msgOp:getFuncid()
    local fixedLenOp = LFixedLen:new(funcid, msg)
    local req = fixedLenOp:parseReq()
        
    local ans = self:businessLogic(req)

    local ansStr = fixedLenOp:generateAnsStr(ans)
    --print('['..ansStr..']')
    return ansStr
end

return QRY00001
