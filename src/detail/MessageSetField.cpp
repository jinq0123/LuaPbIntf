#include "MessageSetField.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include "google/protobuf/descriptor.pb.h"  // for map_entry()

using namespace LuaIntf;
using namespace google::protobuf;
using namespace std;

void FieldSetter::SetField(Message* pMsg,
    const std::string& sField, const LuaRef& luaValue)
{
    assert(pMsg);
    const Descriptor* pDesc = pMsg->GetDescriptor();
    assert(pDesc);
    const FieldDescriptor* pField = pDesc->FindFieldByName(sField);
    if (!pField)
        throw LuaException("Message " + pMsg->GetTypeName() + " has no field: " + sField);
    const Reflection* pRefl = pMsg->GetReflection();
    if (!pRefl)
        throw LuaException("Message has no reflection.");

    if (pField->is_repeated())
    {
        throw LuaException("Setting repeated field is to be implemented.");
        // XXX return SetRepeatedField(*pMsg, pField, luaValue);
    }

    using Fd = FieldDescriptor;
    Fd::CppType eCppType = pField->cpp_type();
    switch (eCppType)
    {
    case Fd::CPPTYPE_INT32:
        pRefl->SetInt32(pMsg, pField, luaValue.toValue<int32>());
        return;
    case Fd::CPPTYPE_INT64:
        pRefl->SetInt64(pMsg, pField, luaValue.toValue<int64>());
        return;
    case Fd::CPPTYPE_UINT32:
        pRefl->SetUInt32(pMsg, pField, luaValue.toValue<uint32>());
        return;
    case Fd::CPPTYPE_UINT64:
        pRefl->SetUInt64(pMsg, pField, luaValue.toValue<uint64>());
        return;
    case Fd::CPPTYPE_DOUBLE:
        pRefl->SetDouble(pMsg, pField, luaValue.toValue<double>());
        return;
    case Fd::CPPTYPE_FLOAT:
        pRefl->SetFloat(pMsg, pField, luaValue.toValue<float>());
        return;
    case Fd::CPPTYPE_BOOL:
        pRefl->SetBool(pMsg, pField, luaValue.toValue<bool>());
        return;
    case Fd::CPPTYPE_ENUM:
        // XXX Support enum name
        pRefl->SetEnumValue(pMsg, pField, luaValue.toValue<int>());
        return;
    case Fd::CPPTYPE_STRING:
        pRefl->SetString(pMsg, pField, luaValue.toValue<string>());
        return;
    case Fd::CPPTYPE_MESSAGE:
        // XXX
    default:
        break;
    }
    // Unknown field type CPPTYPE_UNKNOWN of Message.Field
    throw LuaException(string("Unknown field type ") +
        pField->CppTypeName(eCppType) + " of " +
        pMsg->GetTypeName() + "." + sField);
}
