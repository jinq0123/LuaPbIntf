#include "MessageSetter.h"

#include "Config.h"  // for LUA_HAS_64BIT_INTEGER

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include <google/protobuf/descriptor.pb.h>  // for map_entry()

#include <iostream>  // for cout

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
#if LUA_HAS_64BIT_INTEGER
        m_pRefl->SetUInt64(&m_rMsg, pField, luaValue.toValue<uint64>());
#else  // To support Lua5.1/5.2.
        m_pRefl->SetUInt64(&m_rMsg, pField, luaValue.toValue<double>());
#endif
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
        // Support enum name.
        m_pRefl->SetEnumValue(&m_rMsg, pField, GetEnumValue(luaValue, *pField));
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
            continue;  // Skip non-string key.
        const string& sKey = key.toValue<string>();
        // std::cout << sKey << std::endl;
        const LuaRef& val = itr.value();
        SetField(sKey, val);
    }
}

// Support map. luaTable is an array or a map in lua table.
void MessageSetter::SetRepeatedField(const FieldDescriptor& field,
    const LuaRef& luaTable)
{
    assert(field.is_repeated());
    if (!luaTable.isTable())
    {
        throw LuaException("Field " + field.full_name() +
            " expects a table but got a " + luaTable.typeName());
    }

    if (field.is_map())
    {
        SetRepeatedMapField(field, luaTable);
        return;
    }

    // non-map
    int len = luaTable.len();
    for (int index = 1; index <= len; ++index)
    {
        const LuaRef& val = luaTable[index];
        AddToRepeatedField(field, val);
    }
}

void MessageSetter::SetRepeatedMapField(const FieldDescriptor& field,
    const LuaRef& luaTable)
{
    assert(field.is_repeated());
    assert(field.is_map());
    assert(luaTable.isTable());

    const auto itrEnd = luaTable.end();
    for (auto itr = luaTable.begin(); itr != itrEnd; ++itr)
    {
        const LuaRef& key = itr.key();
        const LuaRef& val = itr.value();
        AddToMapField(field, key, val);
    }
}

// Add value to repeated field. Non-map-entry.
void MessageSetter::AddToRepeatedField(
    const FieldDescriptor& field, const LuaRef& luaValue)
{
    assert(field.is_repeated());
    assert(!field.is_map());

    using Fd = FieldDescriptor;
    Fd::CppType eCppType = field.cpp_type();
    switch (eCppType)
    {
    case Fd::CPPTYPE_INT32:
        m_pRefl->AddInt32(&m_rMsg, &field, luaValue.toValue<int32>());
        return;
    case Fd::CPPTYPE_INT64:
        m_pRefl->AddInt64(&m_rMsg, &field, luaValue.toValue<int64>());
        return;
    case Fd::CPPTYPE_UINT32:
        m_pRefl->AddUInt32(&m_rMsg, &field, luaValue.toValue<uint32>());
        return;
    case Fd::CPPTYPE_UINT64:
        m_pRefl->AddUInt64(&m_rMsg, &field, luaValue.toValue<uint64>());
        return;
    case Fd::CPPTYPE_DOUBLE:
        m_pRefl->AddDouble(&m_rMsg, &field, luaValue.toValue<double>());
        return;
    case Fd::CPPTYPE_FLOAT:
        m_pRefl->AddFloat(&m_rMsg, &field, luaValue.toValue<float>());
        return;
    case Fd::CPPTYPE_BOOL:
        m_pRefl->AddBool(&m_rMsg, &field, luaValue.toValue<bool>());
        return;
    case Fd::CPPTYPE_ENUM:
        // Support enum name.
        m_pRefl->AddEnumValue(&m_rMsg, &field, GetEnumValue(luaValue, field));
        return;
    case Fd::CPPTYPE_STRING:
        m_pRefl->AddString(&m_rMsg, &field, luaValue.toValue<string>());
        return;
    case Fd::CPPTYPE_MESSAGE:
        if (luaValue.isTable())
        {
            Message* pSubMsg = m_pRefl->AddMessage(&m_rMsg, &field);
            assert(pSubMsg);
            MessageSetter(*pSubMsg).SetMsg(luaValue);
            return;
        }
        throw LuaException("Field " + field.full_name() +
            " expects a table but got a " + luaValue.typeName());
    default:
        break;
    }
    // Unknown field type CPPTYPE_UNKNOWN of Message.Field
    throw LuaException(string("Unknown field type ") +
        field.CppTypeName(eCppType) + " of " + field.full_name());
}

void MessageSetter::AddToMapField(const FieldDescriptor& field,
    const LuaRef& key, const LuaRef& val)
{
    assert(field.is_repeated());
    assert(field.is_map());
    assert(field.cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE);
    Message* pMapEntry = m_pRefl->AddMessage(&m_rMsg, &field);
    assert(pMapEntry);
    MessageSetter setter(*pMapEntry);
    setter.SetField("key", key);
    setter.SetField("value", val);
}  // AddToMapField

// Try to convert lua string to enum value.
// If lua value is not string or not enum string, get int.
int MessageSetter::GetEnumValue(const LuaRef& luaValue,
    const FieldDescriptor& field) const
{
    if (luaValue.type() != LuaIntf::LuaTypeID::STRING)
        return luaValue.toValue<int>();

    std::string sEnum = luaValue.toValue<string>();
    assert(field.cpp_type() == FieldDescriptor::CPPTYPE_ENUM);
    const EnumDescriptor* pEnum = field.enum_type();
    assert(pEnum);
      // Looks up a value by name.  Returns NULL if no such value exists.
    const EnumValueDescriptor* pEnumVal = pEnum->FindValueByName(sEnum);
    if (pEnumVal) return pEnumVal->number();
    return luaValue.toValue<int>();  // "123" -> 123
}
