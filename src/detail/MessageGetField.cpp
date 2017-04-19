#include "MessageGetField.h"

#include <google/protobuf/message.h>  // for Message
#include <LuaIntf/LuaIntf.h>

LuaIntf::LuaRef MessageGetField(
    const google::protobuf::Message* pMsg,
    const std::string& sField)
{
    return LuaIntf::LuaRef();
}

