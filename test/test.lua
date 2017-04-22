local pb = require("luapbintf.luapbintf")

-- test.proto imports common.proto
pb.import_proto_file("test.proto")

assert(pb.message("test.CommonMsg"))
assert(pb.message("test.TestMsg"))

local msg = {}
msg.uid = 12345
assert(msg.uid == 12345)
msg.param = 9876
msg.name = "Jin Qing"
msg.names = {"n1", "n2", "n3"}

--for i = 1, msg.param2:len() do
--    local value = msg.param2:get(i)
--    print("i: " .. i .. " value: " .. value)
--end

--msg.param2[1] = "test"
--print("===== param2: " .. msg.param2:get(1))

--msg.param2:set(2, "test2")
--print("===== param2: " .. msg.param2:get(2))

local sz = pb.encode("test.TestMsg", msg)
assert(#sz)

--local enum_value = pb.getEnumValue("lm.Cmd", "CMD_TYPE_USER")
--print(enum_value)

local msg2 = pb.decode("test.TestMsg", sz)
assert(msg2.uid == 12345)

print("Test OK!")
