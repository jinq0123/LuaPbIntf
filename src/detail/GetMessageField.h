#ifndef DETAIL_GETMESSAGEFIELD_H
#define DETAIL_GETMESSAGEFIELD_H

#include <string>

struct lua_State;

namespace google {
namespace protobuf {
class Message;
class FieldDescriptor;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

LuaIntf::LuaRef GetMessageField(lua_State& rLuaState,
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor& field);

#endif  // DETAIL_GETMESSAGEFIELD_H
