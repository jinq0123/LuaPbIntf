-- luapbintf.lua

local M = {}

local c = require("luapbintf.c")

M.test				= c.test
M.add_proto_path	= c.add_proto_path
M.map_path			= c.map_path
M.import_proto_file	= c.import_proto_file
M.Message			= c.Message

function M.encode(msg_type_name, t)
    assert("string" == type(msg_type_name))
    assert("table" == type(t))
    -- XXX
end  -- M.encode()

-- decode(msg_type_name) will return a default message.
function M.decode(msg_type_name, s)
    assert("string" == type(msg_type_name))
    assert(nil == s or "string" == type(s))
    -- XXX
end  -- M.encode()

return M
