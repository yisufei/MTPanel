

function displayTable(t)
    for k,v in pairs(t) do
        if type(v) == 'table' then
            displayTable(v)
        else
            print(k..':'..v)
        end
    end
end

function trim(s)
    return s:match("^%s*(.-)%s*$")
end

function extend(s, e)
    for k, v in pairs(e) do
        s[k] = v
        --print('k:'..k)
    end
end
