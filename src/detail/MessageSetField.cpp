#include "MessageSetField.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include "google/protobuf/descriptor.pb.h"  // for map_entry()

using namespace LuaIntf;
using namespace google::protobuf;
using namespace std;

namespace {

inline SetFieldResult ErrorResult(const std::string& sError)
{
    return make_tuple(false, sError);
}

inline SetFieldResult OkResult()
{
    return make_tuple(true, "");
}

}  // namespace

SetFieldResult MessageSetField(Message* pMsg,
    const std::string& sField, const LuaRef& luaRef)
{
    assert(pMsg);
    const Descriptor* pDesc = pMsg->GetDescriptor();
    assert(pDesc);
    const FieldDescriptor* pField = pDesc->FindFieldByName(sField);
    if (!pField)
        return ErrorResult("Message " + pMsg->GetTypeName() + " has no field: " + sField);
    const Reflection* pRefl = pMsg->GetReflection();
    if (!pRefl)
        return ErrorResult("Message has no reflection.");

    if (pField->is_repeated())
    {
        return ErrorResult("Setting repeated field is to be implemented.");
        // XXX return SetRepeatedField(*pMsg, pField, luaRef);
    }

    using Fd = FieldDescriptor;
    Fd::CppType eCppType = pField->cpp_type();
    switch (eCppType)
    {
    case Fd::CPPTYPE_INT32:
        pRefl->SetInt32(pMsg, pField, luaRef.toValue<int32>());
        return OkResult();
    case Fd::CPPTYPE_INT64:
        pRefl->SetInt64(pMsg, pField, luaRef.toValue<int64>());
        return OkResult();
    case Fd::CPPTYPE_UINT32:
        pRefl->SetUInt32(pMsg, pField, luaRef.toValue<uint32>());
        return OkResult();
    case Fd::CPPTYPE_UINT64:
        pRefl->SetUInt64(pMsg, pField, luaRef.toValue<uint64>());
        return OkResult();
    case Fd::CPPTYPE_DOUBLE:
        pRefl->SetDouble(pMsg, pField, luaRef.toValue<double>());
        return OkResult();
    case Fd::CPPTYPE_FLOAT:
        pRefl->SetFloat(pMsg, pField, luaRef.toValue<float>());
        return OkResult();
    case Fd::CPPTYPE_BOOL:
        pRefl->SetBool(pMsg, pField, luaRef.toValue<bool>());
        return OkResult();
    case Fd::CPPTYPE_ENUM:
        // XXX Support enum name
        pRefl->SetEnumValue(pMsg, pField, luaRef.toValue<int>());
        return OkResult();
    case Fd::CPPTYPE_STRING:
        pRefl->SetString(pMsg, pField, luaRef.toValue<string>());
        return OkResult();
    case Fd::CPPTYPE_MESSAGE:
        // XXX
    default:
        break;
    }
    // Unknown field type CPPTYPE_UNKNOWN of Message.Field
    return ErrorResult(string("Unknown field type ") +
        pField->CppTypeName(eCppType) + " of " +
        pMsg->GetTypeName() + "." + sField);
}
