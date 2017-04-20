#include "MessageGetField.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message

using namespace LuaIntf;
using namespace google::protobuf;
using namespace std;

namespace {

tuple<LuaRef, string>
ErrorResult(lua_State* L, const string& sError)
{
    return make_tuple(LuaRef(L, nullptr), sError);
}

template <typename T>
tuple<LuaRef, string>
ValueResult(lua_State* L, const T& value)
{
    return make_tuple(LuaRef::fromValue(L, value), "");
}

tuple<LuaRef, string>
MessageResult(lua_State* L, const Message& msg)
{
    std::shared_ptr<Message> pMsg(msg.New());
    pMsg->CopyFrom(msg);
    return ValueResult(L, pMsg);
}

}  // namespace

tuple<LuaRef, string>
MessageGetField(lua_State* L,
    const Message& msg,
    const std::string& sField)
{
    assert(L);

    const Descriptor* pDesc = msg.GetDescriptor();
    assert(pDesc);
    const FieldDescriptor* pField = pDesc->FindFieldByName(sField);
    if (!pField)
        return ErrorResult(L, "Message " + msg.GetTypeName() + " has no field: " + sField);
    const Reflection* pRefl = msg.GetReflection();
    if (!pRefl)
        return ErrorResult(L, "Message has no reflection.");

    if (pField->is_repeated())
    {
        // XXX
        return ErrorResult(L, "Repeated field is to be implemented.");
    }

    using Fd = FieldDescriptor;
    Fd::CppType eCppType = pField->cpp_type();
    switch (eCppType)
    {
    case Fd::CPPTYPE_INT32:
        return ValueResult(L, pRefl->GetInt32(msg, pField));
    case Fd::CPPTYPE_INT64:
        return ValueResult(L, pRefl->GetInt64(msg, pField));
    case Fd::CPPTYPE_UINT32:
        return ValueResult(L, pRefl->GetUInt32(msg, pField));
    case Fd::CPPTYPE_UINT64:
        return ValueResult(L, pRefl->GetUInt64(msg, pField));
    case Fd::CPPTYPE_DOUBLE:
        return ValueResult(L, pRefl->GetDouble(msg, pField));
    case Fd::CPPTYPE_FLOAT:
        return ValueResult(L, pRefl->GetFloat(msg, pField));
    case Fd::CPPTYPE_BOOL:
        return ValueResult(L, pRefl->GetBool(msg, pField));
    case Fd::CPPTYPE_ENUM:
        return ValueResult(L, pRefl->GetEnumValue(msg, pField));
    case Fd::CPPTYPE_STRING:
        return ValueResult(L, pRefl->GetString(msg, pField));
    case Fd::CPPTYPE_MESSAGE:
        return MessageResult(L, pRefl->GetMessage(msg, pField));
    default:
        return ErrorResult(L, "Unknown field type: " + msg.GetTypeName() + "." + sField);
    }
    assert(false);
    return ErrorResult(L, "");
}

