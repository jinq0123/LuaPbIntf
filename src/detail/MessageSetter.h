#ifndef DETAIL_MESSAGESETFIELD_H
#define DETAIL_MESSAGESETFIELD_H

#include <string>

class LuaPbIntfImpl;

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

class FieldSetter final
{
public:
    explicit FieldSetter(const LuaPbIntfImpl& luaPbIntfImpl)
        : m_luaPbIntfImpl(luaPbIntfImpl)
    {
    }

public:
    using Message = google::protobuf::Message;
    using LuaRef = LuaIntf::LuaRef;
    using string = std::string;

    void SetMsgField(Message& rMsg, const string& sField, const LuaRef& luaValue);
    void SetMsg(Message& rMsg, const LuaRef& luaTable);

private:
    // To make Encoder for sub-message.
    const LuaPbIntfImpl& m_luaPbIntfImpl;
};  // class FieldSetter

#endif  // DETAIL_MESSAGESETFIELD_H
