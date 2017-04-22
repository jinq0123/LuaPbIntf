-- luapbintf/detail/message_metatable.lua
-- Message metatable.

local M = {}

local c = require("luapbintf.c")

-- t must be a message proxy table.
function M.__index(t, k)
    assert(k ~= c)
    local c_msg = t[c]  -- Internal C++ Message object.
    return c_msg:get_field(k)
end

return M
