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
Result(lua_State* L, const T& value)
{
    return make_tuple(LuaRef::fromValue(L, value), "");
}

}  // namespace

tuple<LuaRef, string>
MessageGetField(lua_State* L,
    const google::protobuf::Message& msg,
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
        return Result(L, pRefl->GetInt32(msg, pField));
    case Fd::CPPTYPE_INT64:
        return Result(L, pRefl->GetInt64(msg, pField));
    case Fd::CPPTYPE_UINT32:
        return Result(L, pRefl->GetUInt32(msg, pField));
    case Fd::CPPTYPE_UINT64:
        return Result(L, pRefl->GetUInt64(msg, pField));
    case Fd::CPPTYPE_DOUBLE:
        return Result(L, pRefl->GetDouble(msg, pField));
    case Fd::CPPTYPE_FLOAT:
        return Result(L, pRefl->GetFloat(msg, pField));
    case Fd::CPPTYPE_BOOL:
        return Result(L, pRefl->GetBool(msg, pField));
    case Fd::CPPTYPE_ENUM:
        return Result(L, pRefl->GetEnumValue(msg, pField));
    case Fd::CPPTYPE_STRING:
        return Result(L, pRefl->GetString(msg, pField));
    case Fd::CPPTYPE_MESSAGE:
        return ErrorResult(L, "Message field is to be implemented.");
    default:
        return ErrorResult(L, "Unknown field type: " + msg.GetTypeName() + "." + sField);
    }
    return ErrorResult(L, "");
}

