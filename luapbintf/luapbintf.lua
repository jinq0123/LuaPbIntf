-- luapbintf/luapbintf.lua

local M = {}

local c = require("luapbintf.c")
local msg_mt = require("luapbintf.detail.message_metatable");

M.test				= c.test
M.add_proto_path	= c.add_proto_path
M.map_path			= c.map_path
M.import_proto_file	= c.import_proto_file

-- Encode message table to string.
function M.encode(msg_type_name, t)
    assert("string" == type(msg_type_name))
    assert("table" == type(t))
    local c_msg = c.Message(msg_type_name)
    for k,v in pairs(t) do
        -- XXX if v is table...
        if "string" == type(k) then
            c_msg:set_field(k, v)
        end
    end
    return c_msg:serialize()
end  -- M.encode()

-- Decode string to message proxy table.
function M.decode(msg_type_name, s)
    assert("string" == type(msg_type_name))
    assert("string" == type(s))
    local msg = M.message(msg_type_name)
    local c_msg = msg[c]  -- C++ Message object
    c_msg:parse(s)
    return msg
end  -- M.decode()

-- Return a default message(proxy table).
function M.message(msg_type_name)
    assert("string" == type(msg_type_name))
    -- C++ Message object.
    local c_msg = assert(c.Message(msg_type_name))
    local msg = {}
    msg[c] = c_msg  -- private
    setmetatable(msg, msg_mt)
    return msg
end  -- message()

return M
