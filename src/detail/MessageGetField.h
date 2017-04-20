#ifndef DETAIL_MESSAGEGETFIELD_H
#define DETAIL_MESSAGEGETFIELD_H

#include <string>

struct lua_State;

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

using FieldResult = std::tuple<LuaIntf::LuaRef, std::string>;

// Returns (LuaRef, "") or (nil, error_string)
FieldResult MessageGetField(lua_State* L,
    const google::protobuf::Message& msg,
    const std::string& sFields);

#endif  // DETAIL_MESSAGEGETFIELD_H
