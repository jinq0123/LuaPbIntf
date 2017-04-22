#include "Encoder.h"

#include "LuaPbIntfImpl.h"  // for MakeSharedMessage()
#include "MessageSptr.h"

#include <LuaIntf/LuaIntf.h>
#include <LuaIntf/LuaState.h>  // for LuaException
#include <google/protobuf/message.h>  // for Message

#include <iostream>

namespace LuaIntf
{
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
}

using LuaIntf::LuaRef;
using LuaIntf::LuaException;
using google::protobuf::Message;
using std::string;

namespace {

// The key to index C++ Message object in proxy table.
LuaRef GetMessageKey(lua_State* L)
{
    assert(L);

    // In lua: c = require("luapbintf.c")
    LuaRef require(L, "require");
    LuaRef c = require.call<LuaRef>("luapbintf.c");
    assert(c.isTable());
    return c;
}

// Get MessageSptr in table.
MessageSptr GetMessageSptrInTable(const LuaRef& luaTable)
{
    assert(luaTable.isTable());
    // Key is always the same.
    static LuaRef s_key = GetMessageKey(luaTable.state());
    LuaRef c_msg = luaTable.rawget(s_key);
    if (!c_msg) return nullptr;
    return c_msg.toValue<MessageSptr>();
}

}  // namespace

// luaTable may be a normal lua table,
// a message proxy table which has a C++ MessageSptr object,
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
    MessageSptr pMsg = GetMessageSptr(sMsgTypeName, luaTable);
    assert(pMsg);
    // XXX
    return nullptr;
}  // EncodeToMessage()

// Get MessageSptr in table or make a new one.
MessageSptr Encoder::GetMessageSptr(const string& sMsgTypeName,
    const LuaRef& luaTable) const
{
    MessageSptr pMsg = GetMessageSptrInTable(luaTable);
    if (!pMsg)
        return m_luaPbIntfImpl.MakeSharedMessage(sMsgTypeName);
    if (pMsg->GetTypeName() == sMsgTypeName)
        return pMsg;

    throw LuaException("Message type mismatch (" + sMsgTypeName +
        " expected, got " + pMsg->GetTypeName() + ").");
}

