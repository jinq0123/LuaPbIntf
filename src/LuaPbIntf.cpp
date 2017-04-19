#include "detail/LuaPbIntfImpl.h"

#include <LuaIntf/LuaIntf.h>

#include <iostream>

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
    using namespace LuaIntf;

    auto pImpl = std::make_shared<LuaPbIntfImpl>();
    LuaRef mod = LuaRef::createTable(L);
    LuaBinding(mod)
        .addFunction("test", &test)
        // add_proto_path() must before compile_proto_file()
        .addFunction("add_proto_path",
            [pImpl](const string& sProtoPath) {
                pImpl->AddProtoPath(sProtoPath);
            })
        // map_path() must before compile_proto_file()
        .addFunction("map_path",
            [pImpl](const string& sVirtualPath, const string& sDiskPath) {
                pImpl->MapPath(sVirtualPath, sDiskPath);
            })
        // Input file must be relative to proto paths.
        .addFunction("compile_proto_file",
            [pImpl](const string& sProtoFile) {
                return pImpl->CompileProtoFile(sProtoFile);
            })
        ;
    mod.pushToStack();
    return 1;
}
