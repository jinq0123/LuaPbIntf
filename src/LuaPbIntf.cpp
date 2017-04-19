#include "detail/LuaPbIntfImpl.h"

#include <google/protobuf/message.h>
#include <LuaIntf/LuaIntf.h>

#include <iostream>  // for cout
#include <memory>  // for shared_ptr

namespace LuaIntf
{
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
}

namespace {

void test()
{
    std::cout << "test...\n";
}

}  // namespace

extern "C"
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
__declspec(dllexport)
#endif
int luaopen_luapbintf(lua_State* L)
{
    using string = std::string;
    using Message = google::protobuf::Message;
    using namespace LuaIntf;

    auto pImpl = std::make_shared<LuaPbIntfImpl>();
    LuaRef mod = LuaRef::createTable(L);
    LuaBinding(mod)
        .addFunction("test", &test)

        .addFunction("add_proto_path",
            [pImpl](const string& sProtoPath) {
                pImpl->AddProtoPath(sProtoPath);
            })
        .addFunction("map_path",
            [pImpl](const string& sVirtualPath, const string& sDiskPath) {
                pImpl->MapPath(sVirtualPath, sDiskPath);
            })
        // Input file must be relative to proto paths.
        .addFunction("compile_proto_file",
            [pImpl](const string& sProtoFile) {
                return pImpl->CompileProtoFile(sProtoFile);
            })

        .beginClass<Message>("Message")
            .addFactory([pImpl](const std::string& sTypeName) {
                    return pImpl->MakeSharedMessage(sTypeName);  // maybe nullptr
                })
            .addFunction("debug_string", &Message::DebugString)
            .addFunction("short_debug_string", &Message::ShortDebugString)
            .addFunction("utf8_debug_string", &Message::Utf8DebugString)
            .addFunction("utf8_debug_string", &Message::Utf8DebugString)
            .addProperty("type_name", &Message::GetTypeName)
        .endClass()

        ;
    mod.pushToStack();
    return 1;
}
