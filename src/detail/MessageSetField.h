#ifndef DETAIL_MESSAGESETFIELD_H
#define DETAIL_MESSAGESETFIELD_H

#include <string>

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

void MessageSetField(google::protobuf::Message* pMsg,
    const std::string& sFields, const LuaIntf::LuaRef& luaRef);

#endif  // DETAIL_MESSAGESETFIELD_H
