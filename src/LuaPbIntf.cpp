#include "detail/LuaPbIntfImpl.h"

#include <google/protobuf/message.h>
#include <LuaIntf/LuaIntf.h>

#include <iostream>  // for cout
#include <memory>  // for shared_ptr

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
    using std::string;
    using google::protobuf::Message;
    using LuaIntf::LuaRef;

    auto pImpl = std::make_shared<LuaPbIntfImpl>();
    LuaRef mod = LuaRef::createTable(L);
    LuaIntf::LuaBinding(mod)
        .addFunction("test", &test)

        // See doc/reference.md
        .addFunction("add_proto_path",
            [pImpl](const string& sProtoPath) {
                pImpl->AddProtoPath(sProtoPath);
            })
        .addFunction("map_path",
            [pImpl](const string& sVirtualPath, const string& sDiskPath) {
                pImpl->MapPath(sVirtualPath, sDiskPath);
            })
        .addFunction("import_proto_file",
            [pImpl](const string& sProtoFile) {
                pImpl->ImportProtoFile(sProtoFile);
            })
        .addFunction("encode",
            [pImpl](const string& sMsgTypeName, const LuaRef& luaTable) {
                return pImpl->Encode(sMsgTypeName, luaTable);
            })
        .addFunction("decode",
            [L, pImpl](const string& sMsgTypeName, const string& sData) {
                return pImpl->Decode(L, sMsgTypeName, sData);
            })
        .addFunction("get_rpc_input_name",
            [pImpl](const string& sServiceName, const string& sMethodName) {
                return pImpl->GetRpcInputName(sServiceName, sMethodName);
            })
        .addFunction("get_rpc_output_name",
            [pImpl](const string& sServiceName, const string& sMethodName) {
                return pImpl->GetRpcOutputName(sServiceName, sMethodName);
            })
        .addFunction("get_service_descriptor",
            [pImpl](const string& sServiceName) {
                return pImpl->GetServiceDescriptor(sServiceName);
            })
        ;  // LuaBinding(mod)

    mod.pushToStack();
    return 1;
}
