#include "MessageSetter.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include <google/protobuf/descriptor.pb.h>  // for map_entry()

#include <iostream>

using namespace google::protobuf;
using LuaIntf::LuaException;

MessageSetter::MessageSetter(Message& rMsg)
    : m_rMsg(rMsg),
    m_pDesc(m_rMsg.GetDescriptor()),
    m_pRefl(rMsg.GetReflection())
{
    if (!m_pDesc)
    {
        throw LuaException("Message " + rMsg.GetTypeName() +
            " has no descriptor.");
    }
    if (!m_pRefl)
    {
        throw LuaException("Message " + rMsg.GetTypeName() +
            " has no reflection.");
    }
}

void MessageSetter::SetField(const string& sField, const LuaRef& luaValue)
{
    assert(m_pDesc);
    assert(m_pRefl);
    const FieldDescriptor* pField = m_pDesc->FindFieldByName(sField);
    if (!pField)
    {
        throw LuaException("Message " + m_rMsg.GetTypeName() +
            " has no field: " + sField);
    }
    if (pField->is_repeated())
    {
        SetRepeatedField(*pField, luaValue);
        return;
    }

    using Fd = FieldDescriptor;
    Fd::CppType eCppType = pField->cpp_type();
    switch (eCppType)
    {
    case Fd::CPPTYPE_INT32:
        m_pRefl->SetInt32(&m_rMsg, pField, luaValue.toValue<int32>());
        return;
    case Fd::CPPTYPE_INT64:
        m_pRefl->SetInt64(&m_rMsg, pField, luaValue.toValue<int64>());
        return;
    case Fd::CPPTYPE_UINT32:
        m_pRefl->SetUInt32(&m_rMsg, pField, luaValue.toValue<uint32>());
        return;
    case Fd::CPPTYPE_UINT64:
        m_pRefl->SetUInt64(&m_rMsg, pField, luaValue.toValue<uint64>());
        return;
    case Fd::CPPTYPE_DOUBLE:
        m_pRefl->SetDouble(&m_rMsg, pField, luaValue.toValue<double>());
        return;
    case Fd::CPPTYPE_FLOAT:
        m_pRefl->SetFloat(&m_rMsg, pField, luaValue.toValue<float>());
        return;
    case Fd::CPPTYPE_BOOL:
        m_pRefl->SetBool(&m_rMsg, pField, luaValue.toValue<bool>());
        return;
    case Fd::CPPTYPE_ENUM:
        // XXX Support enum name
        m_pRefl->SetEnumValue(&m_rMsg, pField, luaValue.toValue<int>());
        return;
    case Fd::CPPTYPE_STRING:
        m_pRefl->SetString(&m_rMsg, pField, luaValue.toValue<string>());
        return;
    case Fd::CPPTYPE_MESSAGE:
        if (luaValue.isTable())
        {
            Message* pSubMsg = m_pRefl->MutableMessage(&m_rMsg, pField);
            assert(pSubMsg);
            MessageSetter(*pSubMsg).SetMsg(luaValue);
            return;
        }
        throw LuaException("Field " + pField->full_name() +
            " expects a table but got a " + luaValue.typeName());
    default:
        break;
    }
    // Unknown field type CPPTYPE_UNKNOWN of Message.Field
    throw LuaException(string("Unknown field type ") +
        pField->CppTypeName(eCppType) + " of " + pField->full_name());
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

// XXX support map.
void MessageSetter::SetRepeatedField(const FieldDescriptor& field,
    const LuaRef& luaValue)
{
    assert(field.is_repeated());
    if (!luaValue.isTable())
    {
        throw LuaException("Field " + field.full_name() +
            " expects a table but got a " + luaValue.typeName());
    }
    // XXX
}
