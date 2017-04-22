#include "Encoder.h"

#include "MessageSptr.h"

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/message.h>  // for Message

#include <iostream>

namespace LuaIntf
{
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
}

using LuaIntf::LuaRef;
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

// Get tbl[c] which is a MessageSptr.
MessageSptr GetMessageSptrFromTable(const LuaRef& tbl)
{
    assert(tbl.isTable());
    static LuaRef s_key = GetMessageKey(tbl.state());
    LuaRef c_msg = tbl.rawget(s_key);
    if (!c_msg) return nullptr;
    return c_msg.toValue<MessageSptr>();
}

MessageSptr EncodeToMessage(const string& sMsgTypeName, const LuaRef& tbl)
{
    MessageSptr pMsg = GetMessageSptrFromTable(tbl);
    if (pMsg)
    {
    }
}

}  // namespace

namespace Encoder {

// tbl may be a message proxy table which has a C++ Message object,
//  or a normal table without C++ Message object.
string Encode(const string& sMsgTypeName, const LuaRef& tbl)
{
    tbl.checkTable();  // Bad argument #-1 to 'encode' (table expected, got number)
    MessageSptr pMsg = EncodeToMessage(sMsgTypeName, tbl);
    assert(pMsg);
    return pMsg->SerializeAsString();
}  // Encode()

}  // namespace Encoder
