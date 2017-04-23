#include "MessageSetter.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include <google/protobuf/descriptor.pb.h>  // for map_entry()

#include <iostream>

using namespace google::protobuf;
using LuaIntf::LuaException;

void MessageSetter::SetField(const string& sField, const LuaRef& luaValue)
{
    const Descriptor* pDesc = m_rMsg.GetDescriptor();
    assert(pDesc);
    const FieldDescriptor* pField = pDesc->FindFieldByName(sField);
    if (!pField)
        throw LuaException("Message " + m_rMsg.GetTypeName() + " has no field: " + sField);
    const Reflection* pRefl = m_rMsg.GetReflection();
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
        pRefl->SetInt32(&m_rMsg, pField, luaValue.toValue<int32>());
        return;
    case Fd::CPPTYPE_INT64:
        pRefl->SetInt64(&m_rMsg, pField, luaValue.toValue<int64>());
        return;
    case Fd::CPPTYPE_UINT32:
        pRefl->SetUInt32(&m_rMsg, pField, luaValue.toValue<uint32>());
        return;
    case Fd::CPPTYPE_UINT64:
        pRefl->SetUInt64(&m_rMsg, pField, luaValue.toValue<uint64>());
        return;
    case Fd::CPPTYPE_DOUBLE:
        pRefl->SetDouble(&m_rMsg, pField, luaValue.toValue<double>());
        return;
    case Fd::CPPTYPE_FLOAT:
        pRefl->SetFloat(&m_rMsg, pField, luaValue.toValue<float>());
        return;
    case Fd::CPPTYPE_BOOL:
        pRefl->SetBool(&m_rMsg, pField, luaValue.toValue<bool>());
        return;
    case Fd::CPPTYPE_ENUM:
        // XXX Support enum name
        pRefl->SetEnumValue(&m_rMsg, pField, luaValue.toValue<int>());
        return;
    case Fd::CPPTYPE_STRING:
        pRefl->SetString(&m_rMsg, pField, luaValue.toValue<string>());
        return;
    case Fd::CPPTYPE_MESSAGE:
        if (luaValue.isTable())
        {
            Message* pSubMsg = pRefl->MutableMessage(&m_rMsg, pField);
            assert(pSubMsg);
            MessageSetter(*pSubMsg).SetMsg(luaValue);
            return;
        }
        throw LuaException(m_rMsg.GetTypeName() + "." + sField +
            " expects a table but got a " + luaValue.typeName());
    default:
        break;
    }
    // Unknown field type CPPTYPE_UNKNOWN of Message.Field
    throw LuaException(string("Unknown field type ") +
        pField->CppTypeName(eCppType) + " of " +
        m_rMsg.GetTypeName() + "." + sField);
}

void MessageSetter::SetMsg(const LuaRef& luaTable)
{
    assert(luaTable.isTable());
    const auto itrEnd = luaTable.end();
    for (auto itr = luaTable.begin(); itr != itrEnd; ++itr)
    {
        const LuaRef& key = itr.key();
        if (LuaIntf::LuaTypeID::STRING != key.type())
            continue;
        const string& sKey = key.toValue<string>();
        std::cout << sKey << std::endl;  // DEL
        const LuaRef& val = itr.value();
        SetField(sKey, val);
    }
}
