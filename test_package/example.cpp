#include "lua.h"

extern "C"
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
__declspec(dllimport)
#endif
int luaopen_luapbintf(lua_State* L);

int main() {
    int (*pFun)(lua_State*);
    pFun = luaopen_luapbintf;
    return 0;
}
