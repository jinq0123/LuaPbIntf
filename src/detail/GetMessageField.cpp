#include "GetMessageField.h"

#include "MessageSptr.h"  // for MessageSptr

#include <LuaIntf/LuaIntf.h>
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/message.h>  // for Message
#include "google/protobuf/descriptor.pb.h"  // for map_entry()

using namespace LuaIntf;
using namespace google::protobuf;
using namespace std;

namespace {

MessageSptr MakeSharedMessage(const Message& msg)
{
    MessageSptr pMsg(msg.New());
    pMsg->CopyFrom(msg);
    return pMsg;
}

inline LuaRef MessageResult(lua_State* L, const Message& msg)
{
    return LuaRefValue(L, MakeSharedMessage(msg));
}

// Set lua table element value.
// Use SetTblMessageElement() if element is a Message.
template <typename T>
void SetTblValueElement(LuaRef& rTbl, int index, const T& value)
{
    rTbl.set(index, value);
}

// Set lua table element to Message.
// Support map entry message.
void SetTblMessageElement(LuaRef& rTbl, int index, const Message& msg)
{
    const Descriptor* pDesc = msg.GetDescriptor();
    assert(pDesc);
    bool isMap = pDesc->options().map_entry();
    if (!isMap)
    {
        rTbl.set(index, MakeSharedMessage(msg));
        return;
    }

    // XXX
}

// XXX Use RepeatedFieldRef

// Get repeated field element and insert it to lua table.
// Map field is supported.
// Returns empty if success.


}  // namespace

