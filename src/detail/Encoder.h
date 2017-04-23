#ifndef DETAIL_ENCODER_H
#define DETAIL_ENCODER_H

#include "MessageSptr.h"

#include <string>

class LuaPbIntfImpl;

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

class Encoder final
{
public:
    explicit Encoder(const LuaPbIntfImpl& luaPbIntfImpl)
        : m_luaPbIntfImpl(luaPbIntfImpl)
    {
    }

public:
    using string = std::string;
    using LuaRef = LuaIntf::LuaRef;

    string Encode(const string& sMsgTypeName, const LuaRef& luaTable) const;

private:
    MessageSptr EncodeToMessage(const string& sMsgTypeName,
        const LuaRef& luaTable) const;
    MessageSptr GetMessageSptr(const string& sMsgTypeName,
        const LuaRef& luaTable) const;

private:
    // To make message and to make FielderSetter.
    const LuaPbIntfImpl& m_luaPbIntfImpl;
};  // class Encoder

#endif  // DETAIL_ENCODER_H
