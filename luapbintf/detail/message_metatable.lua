-- luapbintf/detail/message_metatable.lua

local c = require("luapbintf.c")

local function get_field_of_c_message(c_msg, k)
end  -- get_field_of_c_message()

local function set_field_of_c_message(c_msg, k)
end  -- set_field_of_c_message()

local function MessageMetatable(msg_type_name)
    assert("string" == type(msg_type_name))
    local c_message = assert(c.Message(msg_type_name))
    local mt = {}
    mt.__index = function(_, k)
        return get_field_of_c_message(c_message, k)
    end  -- __index()
    mt.__newindex = function(_, k, v)
        set_field_of_c_message(c_message, k, v)
    end  -- __newindex()
    return mt
end  -- new()

return MessageMetatable
