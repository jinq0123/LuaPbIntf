#ifndef DETAIL_MSGTOTBL_H
#define DETAIL_MSGTOTBL_H

struct lua_State;

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

// Message to lua table convertor.
class MsgToTbl final
{
public:
    explicit MsgToTbl(lua_State& rLuaState) : m_rLuaState(rLuaState)
    {
    }

public:
    LuaIntf::LuaRef Convert(const google::protobuf::Message& msg);

private:
    lua_State& m_rLuaState;
};

#endif  // DETAIL_MSGTOTBL_H
