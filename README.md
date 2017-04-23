# LuaPbIntf
Lua Protobuf interface.

Binding Protobuf to Lua53 with lua-intf.

LuaPbIntf is inspired by [luapb](https://github.com/zhanjunxiong/luapb),
  but has been rewritten to take advantage of
  [lua-intf](https://github.com/SteveKChiu/lua-intf),
  which makes the binding easier to use and easier to understand.

LuaPbIntf will dynamic load Protocol Buffer message definition files .proto
  without code generation.

## Build
Use VS2015 build/LuaPbIntf.sln, or build/Makefile.

Output lubpbintf.dll or lubpbintf.so is in build/bin.

## Run test
See [test/READMD.md](test/READMD.md)

## Differ from luapb
* Support proto3
* Support service
* Support to specify the directory in which to search for imports.
* luapb skips some types such as: TYPE_SFIXED32.
  See [issue](https://github.com/zhanjunxiong/luapb/issues/10).
  
## Note
* Enum field value is integer.
* Repeated field or map field is a lua table.
