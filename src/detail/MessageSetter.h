#ifndef DETAIL_MESSAGESETTER_H
#define DETAIL_MESSAGESETTER_H

#include <string>

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

class MessageSetter final
{
public:
    using Message = google::protobuf::Message;
    explicit MessageSetter(Message& rMsg) : m_rMsg(rMsg)
    {
    }

public:
    using LuaRef = LuaIntf::LuaRef;
    using string = std::string;

    void SetField(const string& sField, const LuaRef& luaValue);
    void SetMsg(const LuaRef& luaTable);

private:
    Message& m_rMsg;
};  // class MessageSetter

#endif  // DETAIL_MESSAGESETTER_H
