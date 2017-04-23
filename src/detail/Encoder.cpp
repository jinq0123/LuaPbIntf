#include "Encoder.h"

#include "LuaPbIntfImpl.h"  // for MakeSharedMessage()
#include "MessageSetter.h"
#include "MessageSptr.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/message.h>  // for SerializeAsString()

using namespace LuaIntf;
using std::string;

string Encoder::Encode(const string& sMsgTypeName, const LuaRef& luaTable) const
{
    assert(luaTable.isTable());
    MessageSptr pMsg = EncodeToMessage(sMsgTypeName, luaTable);
    assert(pMsg);
    return pMsg->SerializeAsString();
}  // Encode()

MessageSptr Encoder::EncodeToMessage(const string& sMsgTypeName,
    const LuaRef& luaTable) const
{
    assert(luaTable.isTable());
    MessageSptr pMsg = m_luaPbIntfImpl.MakeSharedMessage(sMsgTypeName);
    assert(pMsg);
    MessageSetter(*pMsg).SetMsg(luaTable);
    return pMsg;
}  // EncodeToMessage()

