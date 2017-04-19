# Init third_party dir

## protobuf
Copy [protobuf](https://github.com/google/protobuf)
as third_party/protobuf.
Check third_party/protobuf/src/google/protobuf/message.h exist.

## lua-intf
Copy [lua-intf](https://github.com/SteveKChiu/lua-intf)
as third_party/lua-intf.
Check third_party/lua-intf/LuaIntf/LuaIntf.h exist.

## lua
Lua5.3 is OK. Lua5.1 or older is not supported. 
Copy lua src as third_party/lua/src. 

## lib
Copy libs to third_party/lib.

* protobuf
* lua

By default, it expects the Lua library to build under C++.
If you really want to use Lua library compiled under C,
you can define `LUAINTF_LINK_LUA_COMPILED_IN_CXX` to 0 in
 [build/premake5.lua](../build/premake5.lua).
<br>See: https://github.com/SteveKChiu/lua-intf

```
defines { "LUAINTF_LINK_LUA_COMPILED_IN_CXX=0" }
```
