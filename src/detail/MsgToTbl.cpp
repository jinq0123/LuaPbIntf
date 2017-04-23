#include "MsgToTbl.h"

#include <LuaIntf/LuaIntf.h>
#include <LuaIntf/LuaState.h>  // for LuaException
#include <google/protobuf/message.h>  // for Message

using namespace LuaIntf;
using namespace google::protobuf;

LuaRef MsgToTbl::Convert(const Message& msg)
{
    LuaRef tbl = LuaRef::createTable(&m_rLuaState);
    // XXX
    return tbl;
}
