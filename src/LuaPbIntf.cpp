#include <LuaIntf/LuaIntf.h>

extern "C"
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
__declspec(dllexport)
#endif
int luaopen_luapbintf(lua_State* L)
{
    using namespace LuaIntf;
    LuaRef mod = LuaRef::createTable(L);
    LuaBinding(mod)
        .addFunction("test", &test)
    mod.pushToStack();
    return 1;
}
