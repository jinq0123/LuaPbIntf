#ifndef DETAIL_MESSAGEGETFIELD_H
#define DETAIL_MESSAGEGETFIELD_H

#include <string>

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

LuaIntf::LuaRef MessageGetField(
    const google::protobuf::Message* pMsg,
    const std::string& sField);

#endif  // DETAIL_MESSAGEGETFIELD_H
