from conans import ConanFile, CMake

class LuapbintfConan(ConanFile):
    name = "luapbintf"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/jinq0123/LuaPbIntf"
    description = "Binding Protobuf 3 to Lua 5.3"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=True"
    generators = "cmake"
    exports_sources = "src/*"
    requires = ("lua-cpp/5.3.4@jinq0123/testing",
                "lua-intf/0.1@jinq0123/testing",
                "protobuf/3.5.1@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        self.run('cmake %s/src %s' % (self.source_folder, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        # self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["luapbintf"]
