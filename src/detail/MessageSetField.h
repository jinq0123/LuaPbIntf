#ifndef DETAIL_MESSAGESETFIELD_H
#define DETAIL_MESSAGESETFIELD_H

#include <string>
#include <tuple>

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

using SetFieldResult = std::tuple<bool, std::string>;

// Returns (true, "") or (false, error_string)
SetFieldResult MessageSetField(
    google::protobuf::Message* pMsg,
    const std::string& sFields,
    const LuaIntf::LuaRef& luaRef);

#endif  // DETAIL_MESSAGESETFIELD_H
