REM Read premake5.lua and generate GNU makefiles and vs2015 solution.
premake5.exe --os=linux gmake
REM or premake5 --os=macosx gmake
premake5.exe vs2015
pause
