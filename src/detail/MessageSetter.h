#ifndef DETAIL_MESSAGESETTER_H
#define DETAIL_MESSAGESETTER_H

#include <string>

namespace google {
namespace protobuf {
class Descriptor;
class FieldDescriptor;
class Message;
class Reflection;
}  // namespace protobuf
}  // namespace google

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

class MessageSetter final
{
public:
    explicit MessageSetter(google::protobuf::Message& rMsg);

public:
    using LuaRef = LuaIntf::LuaRef;
    using string = std::string;

    void SetField(const string& sField, const LuaRef& luaValue);
    void SetMsg(const LuaRef& luaTable);

private:
    using FieldDescriptor = google::protobuf::FieldDescriptor;
    void SetRepeatedField(const FieldDescriptor& field,
        const LuaRef& luaTable);
    void SetRepeatedMapField(const FieldDescriptor& field,
        const LuaRef& luaTable);
    void AddToRepeatedField(const FieldDescriptor& field,
        const LuaRef& luaValue);
    void AddToMapField(const FieldDescriptor& field,
        const LuaRef& key, const LuaRef& val);

private:
    google::protobuf::Message& m_rMsg;
    const google::protobuf::Descriptor* m_pDesc;  // = m_rMsg.GetDescriptor();
    const google::protobuf::Reflection* m_pRefl;  // = m_rMsg.GetReflection();
};  // class MessageSetter

#endif  // DETAIL_MESSAGESETTER_H
