local pb = require("luapbintf")

-- test.proto imports common.proto
pb.import_proto_file("test.proto")

local msg = {
    uid = 12345,
    param = 9876,
    name = "Jin Qing",
    names = {"n1", "n2", "n3"},
    cmd = 10,
    common_msg = {},
}

local sz = pb.encode("test.TestMsg", msg)
assert(#sz)

--local enum_value = pb.getEnumValue("lm.Cmd", "CMD_TYPE_USER")
--print(enum_value)

local msg2 = pb.decode("test.TestMsg", sz)
assert(msg2.uid == 12345)
assert(msg2.name == "Jin Qing")
assert(#msg2.names == 3)
local n3 = msg2.names[3]
-- Maybe reordered.
assert(n3 == "n1" or n3 == "n2" or n3 == "n3")
assert(10 == msg2.cmd)
assert(msg2.common_msg)

print("Test OK!")
