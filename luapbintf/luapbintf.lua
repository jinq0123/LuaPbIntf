-- luapbintf/luapbintf.lua

local M = {}

local c = require("luapbintf.c")
local MsgMt = require("luapbintf.detail.message_metatable");

M.test				= c.test
M.add_proto_path	= c.add_proto_path
M.map_path			= c.map_path
M.import_proto_file	= c.import_proto_file
M.Message			= c.Message

-- Encode message table to string.
function M.encode(msg_type_name, t)
    assert("string" == type(msg_type_name))
    assert("table" == type(t))
    -- XXX
end  -- M.encode()

-- Decode string to message table.
function M.decode(msg_type_name, s)
    assert("string" == type(msg_type_name))
    assert("string" == type(s))
    -- XXX
end  -- M.decode()

--local function get_message_metatable(msg_type_name)
--    local mt = {}
--    mt.c_message = assert(pb.Message(msg_type_name))
--    mt.__index = function(t, k)
--        print("__index", t,k)
--    end
--    mt.__newindex = function(t, k, v)
--        print("__newindex", t, k, v)
--    end
--    return mt
--end  -- get_message_metatable()

-- Return a default message.
function M.message(msg_type_name)
    assert("string" == type(msg_type_name))
    -- message table is a empty table with a metatable,
    --  which has a C++ Message object.
    local ret = {}
    setmetatable(ret, assert(MsgMt(msg_type_name)))
    return ret
end  -- message()

return M
