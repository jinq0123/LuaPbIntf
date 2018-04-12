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
* Able to set the proto path
* luapb skips some types such as: TYPE_SFIXED32.
  See [issue](https://github.com/zhanjunxiong/luapb/issues/10).

## Build

### Build with conan
1. Install [conan](http://docs.conan.io/en/latest/installation.html).
1. Add conan repositories
    * `conan remote add remote_bintray_bincrafters https://api.bintray.com/conan/bincrafters/public-conan`
    * `conan remote add remote_bintray_jinq0123 https://api.bintray.com/conan/jinq0123/test`
1. `conan create . user/channel -o protobuf:static_rt=False --build missing`
    * The result `luapbintf.dll`/`luapbintf.so` is in `~/.conan/data/luapbintf/0.1/user/channel/package/`...
    * Add build settings like:
        `conan create user/channel --build missing -s arch=x86`

### Without conan (depricated)
Init third_party dir first, see [thirdy_party/README.md](thirdy_party/README.md).
Then use premake5 to generate VS2015 sln or Makefile to build. See [premake/premake5.bat](premake/premake5.bat)

On Windows, it is recommended by [google](https://github.com/google/protobuf/tree/master/cmake)
 that you use static protobuf only.
On Linux, you may need to recompile libprotobuf.a with -fPIC, or link libprotobuf.so.

#### Link error
```error LNK2019: unresolved external symbol "void __cdecl lua_settop(struct lua_State *,int)" (?lua_settop@@YAXPAUlua_State@@H@Z)```

Please see [issue #2](https://github.com/jinq0123/LuaPbIntf/issues/2).

By default, it expects the Lua library to build under C++.
If you really want to use Lua library compiled under C, you can define LUAINTF_LINK_LUA_COMPILED_IN_CXX to 0.

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
    local result = pb.decode("test.TestMsg", s)
    assert(result.name == "")
    assert(result.name2 == nil)
    assert(result.msg2)
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
Proto2 extension is not supported, which is replaced by Any type in proto3.

## Reference
See [doc/reference.md](doc/reference.md).
