-- luapbintf/detail/message_metatable.lua

local M = {}

function M.get(msg_type_name)
    assert("string" == type(msg_type_name))
    local c_message = assert(pb.Message(msg_type_name))
    local mt = {}
    mt.__index = function(_, k)
        return get_field_of_c_message(c_message, k)
    end  -- __index()
    mt.__newindex = function(_, k, v)
    end  -- __newindex()
    return msg
end  -- new()

function MessageMetatable.__index(t, k)
end  -- __index()

function MessageMetatable.__newindex(t, k, v)
end  -- __newindex()

return M
