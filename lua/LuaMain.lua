
package.path = package.path..';../conf/?.lua'

require('basic')
LMessageBase = require('LMessageBase')
entryMap = require('business')

function add(a, b)
    return a+b
end

function getModuleName(path)
    local str = string.reverse(path)
    local s, e
    _, s = string.find(str, 'aul.')
    e, _ = string.find(str, '/')
    assert(s ~= nil)
    assert(e ~= nil)
    return string.reverse( string.sub(str, s+1, e-1) )
end

function entry(msg)
    --print('\nin lua entry\n')
    --local msgOp = LMessageBase:new({msg = msg})
    local msgOp = LMessageBase:new()
    --msgOp:setMsgContent(msg)
    msgOp['setMsg'](msgOp, msg)
    local funcid = msgOp:getFuncid()
    --print('funcid:'..funcid)
    local path = entryMap[funcid]['file']
    local module = getModuleName(path)
    local method = entryMap[funcid]['method']
    --print(method)
    --return method
    local c = require(module)
    local o = c:new({msgOp = msgOp})
    --[==[for k,v in pairs(o) do
        print(k, v)
    end ]==]
    return o[method](o)
end

--print(entry("00000110QRY0000112345678                                                                  89xxxxxxxxxxxxxxxjkfljeakfl0"))
