#include "MsgToTbl.h"

#include "GetMessageField.h"

#include <LuaIntf/LuaIntf.h>
#include <LuaIntf/LuaState.h>  // for LuaException
#include <google/protobuf/message.h>  // for Message

using namespace LuaIntf;
using namespace google::protobuf;

LuaRef MsgToTbl::Convert(const Message& msg)
{
    const Descriptor* pDesc = msg.GetDescriptor();
    assert(pDesc);
    const Reflection* pRefl = msg.GetReflection();
    if (!pRefl)
        throw LuaException("Message " + msg.GetTypeName() + " has no reflection.");

    std::vector<const FieldDescriptor*> vFields;
    pRefl->ListFields(msg, &vFields);
    LuaRef tbl = LuaRef::createTable(&m_rLuaState);
    for (const FieldDescriptor* pField : vFields)
    {
        assert(pField);
        tbl[pField->name()] = GetMessageField(m_rLuaState, msg, *pField);
    }

    return tbl;
}
