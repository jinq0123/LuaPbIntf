# LuaPbIntf
Lua Protobuf interface.

Binding Protobuf to Lua with lua-intf, supporting lua53 and proto3.

LuaPbIntf is inspired by [luapb](https://github.com/zhanjunxiong/luapb),
  but has been rewritten to take advantage of
  [lua-intf](https://github.com/SteveKChiu/lua-intf),
  which makes the binding easier to use and easier to understand.

LuaPbIntf will dynamic load Protocol Buffer message definition files .proto
  without code generation.

LuaPbIntf use lua table to present Message,
 `encode()` to encode lua message table to string,
 and `decode()` to decode string to lua message table.

## Differ from luapb
* Support proto3
* Support service
* Support to specify the directory in which to search for imports.
* luapb skips some types such as: TYPE_SFIXED32.
  See [issue](https://github.com/zhanjunxiong/luapb/issues/10).

## Build
Use VS2015 build/LuaPbIntf.sln, or build/Makefile.

Output lubpbintf.dll or lubpbintf.so is in build/bin.

## Run test
See [test/README.md](test/README.md)

## Example

### Encode and Decode
```lua
local pb = require("luapbintf")

pb.import_proto_file("test.proto")

local msg = { uid = 12345 }
local sz = pb.encode("test.TestMsg", msg)

local msg2 = pb.decode("test.TestMsg", sz)
assert(msg2.uid == 12345)
```  

### Rpc
```lua
    assert(pb.get_rpc_input_name("test.Test", "Foo") == "test.TestMsg")
    assert(pb.get_rpc_output_name("test.Test", "Foo") == "test.CommonMsg")
```

### Map field
```lua
local msgs = {}
msgs["k1"] = {}
msgs["k2"] = {}
pb.encode("test.TestMsg", { msgs = msgs })
```

### Oneof field
```lua
    local msg = { msg2 = {} }
    local s = pb.encode("test.TestMsg", msg)
    local msg2 = pb.decode("test.TestMsg", s)
    assert(msg2.name == "")
    assert(msg2.name2 == nil)
    assert(msg2.msg2)
```
 
## Type Convertion

Number will be converted to string if necessary.
```lua
    local msg = { name = 12345 }
    local s = pb.encode("test.TestMsg", msg)
    assert(pb.decode("test.TestMsg", s).name == "12345")
```

Integer will be converted from one type to another.
  Such as from int64 4294967296 to int32 0:
```lua
    local msg = { n32 = 4294967296 + 123 }
    assert(msg.n32 == 4294967296 + 123)
    local s = pb.encode("test.TestMsg", msg)
    local msg2 = pb.decode("test.TestMsg", s)
    assert(msg2.n32 == 123)
```

String can be converted to integer or float:
```lua
    local msg = { d = "12345e-67" }
    local s = pb.encode("test.TestMsg", msg)
    assert(pb.decode("test.TestMsg", s).d == 12345e-67)
```

Can not convert float to integer.
```
> msg = { n32 = 1.1 }
> pb.encode("test.TestMsg", msg)
stdin:1: bad argument #-1 to 'encode' (number has no integer representation)
```

Enum is integer, but input string enum will be converted to integer.
```lua
    local msg = { cmd = "CMD_TYPE_CHECK" }
    local s = pb.encode("test.TestMsg", msg)
    assert(pb.decode("test.TestMsg", s).cmd == 2)
```

Repeated field is a lua array table and it is 
read from index 1 to len, ignoring others.
```lua
    local msg = { names = {"n1", "n2", "n3", [0] = "n1", [100] = "n100"} }
    local s = pb.encode("test.TestMsg", msg)
    local msg2 = pb.decode("test.TestMsg", s)
    assert(#msg2.names == 3)
```

## Proto2 Extension
Proto2 extension is not supported, which is replaced by Ayn type in proto3.

## Reference
See [doc/reference.md](doc/reference.md).
