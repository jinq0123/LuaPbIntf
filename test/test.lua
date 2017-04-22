local pb = require("luapbintf.luapbintf")

pb.import_proto_file("test.proto")

assert(pb.Message("test.Common"))
local msg = pb.Message("test.TestMsg")
msg.uid = 12345
msg.param = 9876
msg.param1 = "zjx"
--print("type: " .. type(msg.param2))
msg.param2:add("first")
msg.param2:add("second")
msg.param2:add("three")

print("uid: " .. msg.uid)
print("param: " .. msg.param)
print("param1: " .. msg.param1)


for i = 1, msg.param2:len() do
    local value = msg.param2:get(i)
    print("i: " .. i .. " value: " .. value)
end

msg.param2[1] = "test"
print("===== param2: " .. msg.param2:get(1))

msg.param2:set(2, "test2")
print("===== param2: " .. msg.param2:get(2))

local sz = pb.serializeToString(msg)
print("sz size is ".. #sz)

local enum_value = pb.getEnumValue("lm.Cmd", "CMD_TYPE_USER")
print(enum_value)

print("cmd value is:" .. msg.cmd)
