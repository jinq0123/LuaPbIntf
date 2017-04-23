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
int luaopen_luapbintf_c(lua_State* L)
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
        .addFunction("import_proto_file",
            [pImpl](const string& sProtoFile) {
                pImpl->ImportProtoFile(sProtoFile);
            })
        .addFunction("encode",
            [pImpl](const string& sMsgTypeName, const LuaRef& luaTable) {
                pImpl->Encode(sMsgTypeName, luaTable);
            })
        .addFunction("decode",
            [L, pImpl](const string& sMsgTypeName, const string& sData) {
                return pImpl->Decode(L, sMsgTypeName, sData);
            })

        //.beginClass<Message>("Message")
        //    .addFactory([pImpl](const string& sTypeName) {
        //            return pImpl->MakeSharedMessage(sTypeName);  // maybe nullptr
        //        })
        //    // XXX New(), MergeFrom(), CopyFrom()
        //    .addFunction("debug_string", &Message::DebugString)
        //    .addFunction("short_debug_string", &Message::ShortDebugString)
        //    .addFunction("utf8_debug_string", &Message::Utf8DebugString)
        //    .addFunction("utf8_debug_string", &Message::Utf8DebugString)
        //    .addPropertyReadOnly("type_name", &Message::GetTypeName)
        //    .addFunction("clear", &Message::Clear)
        //    .addPropertyReadOnly("is_initialized", &Message::IsInitialized)
        //    .addPropertyReadOnly("byte_size", &Message::ByteSizeLong)

        //    // MessageLite API
        //    .addFunction("parse", &Message::ParseFromString)
        //    .addFunction("parse_partial", &Message::ParsePartialFromString)
        //    .addFunction("serialize", &Message::SerializeAsString)
        //    .addFunction("serialize_partial", &Message::SerializePartialAsString)

        //    // Read and modify the fields of the Message dynamically
        //    .addFunction("get_field",
        //        [L](const Message* pMsg, const string& sField) {
        //            return MessageGetField(L, *pMsg, sField);
        //        })
        //    .addFunction("set_field",
        //        [](Message* pMsg, const string& sField,
        //                const LuaRef& luaValue) {
        //            assert(pMsg);
        //            MessageSetter(*pMsg).SetField(sField, luaValue);
        //        })

        //.endClass()  // Message

        ;  // LuaBinding(mod)
    mod.pushToStack();
    return 1;
}
