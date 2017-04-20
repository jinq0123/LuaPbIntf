#include "MessageGetField.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message

using namespace LuaIntf;
using namespace google::protobuf;

std::tuple<LuaIntf::LuaRef, std::string>
ErrorResult(lua_State* L, const std::string& sError)
{
    return std::make_tuple(LuaRef(L, nullptr), sError);
}

std::tuple<LuaIntf::LuaRef, std::string>
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
    const Reflection* pReflection = msg.GetReflection();
    if (!pReflection)
        return ErrorResult(L, "Message has no reflection.");

    return ErrorResult(L, "");
}

