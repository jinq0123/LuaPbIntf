#include "MessageGetField.h"

#include "MessageSptr.h"  // for MessageSptr

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include "google/protobuf/descriptor.pb.h"  // for map_entry()

using namespace LuaIntf;
using namespace google::protobuf;
using namespace std;

namespace {

FieldResult ErrorResult(lua_State* L, const string& sError)
{
    return make_tuple(LuaRef(L, nullptr), sError);
}

template <typename T>
FieldResult ValueResult(lua_State* L, const T& value)
{
    return make_tuple(LuaRef::fromValue(L, value), "");
}

MessageSptr MakeSharedMessage(const Message& msg)
{
    MessageSptr pMsg(msg.New());
    pMsg->CopyFrom(msg);
    return pMsg;
}

FieldResult MessageResult(lua_State* L, const Message& msg)
{
    return ValueResult(L, MakeSharedMessage(msg));
}

// Set lua table element value.
// Use SetTblMessageElement() if element is a Message.
template <typename T>
void SetTblValueElement(LuaRef& rTbl, int index, const T& value)
{
    rTbl.set(index, value);
}

// Set lua table element to Message.
// Support map entry message.
void SetTblMessageElement(LuaRef& rTbl, int index, const Message& msg)
{
    const Descriptor* pDesc = msg.GetDescriptor();
    assert(pDesc);
    bool isMap = pDesc->options().map_entry();
    if (!isMap)
    {
        rTbl.set(index, MakeSharedMessage(msg));
        return;
    }

    // XXX
}

// XXX Use RepeatedFieldRef

// Get repeated field element and insert it to lua table.
// Map field is supported.
// Returns empty if success.
string GetRepeatedFieldElement(const Message& msg,
    const FieldDescriptor* pField, int index, LuaRef& rTbl)
{
    assert(pField);
    assert(pField->is_repeated());
    assert(index >= 0);
    const Reflection* pRefl = msg.GetReflection();
    assert(pRefl);
    assert(index < pRefl->FieldSize(msg, pField));

    using Fd = FieldDescriptor;
    Fd::CppType eCppType = pField->cpp_type();
    switch (eCppType)
    {
    case Fd::CPPTYPE_INT32:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedInt32(msg, pField, index));
        return "";
    case Fd::CPPTYPE_INT64:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedInt64(msg, pField, index));
        return "";
    case Fd::CPPTYPE_UINT32:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedUInt32(msg, pField, index));
        return "";
    case Fd::CPPTYPE_UINT64:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedUInt64(msg, pField, index));
        return "";
    case Fd::CPPTYPE_DOUBLE:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedDouble(msg, pField, index));
        return "";
    case Fd::CPPTYPE_FLOAT:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedFloat(msg, pField, index));
        return "";
    case Fd::CPPTYPE_BOOL:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedBool(msg, pField, index));
        return "";
    case Fd::CPPTYPE_ENUM:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedEnumValue(msg, pField, index));
        return "";
    case Fd::CPPTYPE_STRING:
        SetTblValueElement(rTbl, index,
            pRefl->GetRepeatedString(msg, pField, index));
        return "";
    case Fd::CPPTYPE_MESSAGE:
        // Support map entry element.
        SetTblMessageElement(rTbl, index,
            pRefl->GetRepeatedMessage(msg, pField, index));
        return "";
    default:
        break;
    }
    // Unknown repeated field type CPPTYPE_UNKNOWN of Message.Field
    return string("Unknown repeated field type ") +
        pField->CppTypeName(eCppType) + " of " +
        msg.GetTypeName() + "." + pField->name();
}

// returns (TableRef, "") or (nil, error_string)
// Map is supported.
FieldResult GetRepeatedField(lua_State* L,
    const Message& msg, const FieldDescriptor* pField)
{
    assert(L);
    assert(pField->is_repeated());
    const Reflection* pRefl = msg.GetReflection();
    assert(pRefl);

    LuaRef tbl = LuaRef::createTable(L);
    int nFldSize = pRefl->FieldSize(msg, pField);
    for (int index = 0; index < nFldSize; ++index)
    {
        string sError = GetRepeatedFieldElement(msg, pField, index, tbl);
        if (!sError.empty())
            return ErrorResult(L, sError);
    }
    return make_tuple(tbl, "");
}

}  // namespace

FieldResult MessageGetField(lua_State* L,
    const Message& msg, const std::string& sField)
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
        // returns (TableRef, "") or (nil, error_string)
        return GetRepeatedField(L, msg, pField);
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
        break;
    }
    // Unknown field type CPPTYPE_UNKNOWN of Message.Field
    return ErrorResult(L, string("Unknown field type ") +
        pField->CppTypeName(eCppType) + " of " +
        msg.GetTypeName() + "." + sField);
}

