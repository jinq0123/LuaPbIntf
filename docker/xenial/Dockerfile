# Dockerfile - vkill/luapbintf for Ubuntu Xenial
# https://github.com/vkill/LuaPbIntf/tree/docker-xenial/docker

FROM ubuntu:xenial

MAINTAINER vkill <vkill.net@gmail.com>

#
ARG LUA_VERSION="5.3.4"
ARG PROTOBUF_VERSION="3.3.0"
ARG RESTY_J="1"

# Install apt dependencies
#
RUN \
    DEBIAN_FRONTEND=noninteractive apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        autoconf automake libtool curl make g++ unzip \
        libreadline-dev

# Download and untar/unzip Lua, Protocol Buffers, lua-intf, and LuaPbIntf
#
RUN \
    cd /usr/local/src \
    && curl -fSL -o lua-${LUA_VERSION}.tar.gz http://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz \
    && tar -zxvf /usr/local/src/lua-${LUA_VERSION}.tar.gz -C /usr/local/src \
    && curl -fSL -o protobuf-cpp-${PROTOBUF_VERSION}.zip https://github.com/google/protobuf/releases/download/v${PROTOBUF_VERSION}/protobuf-cpp-${PROTOBUF_VERSION}.zip \
    && unzip /usr/local/src/protobuf-cpp-${PROTOBUF_VERSION}.zip -d /usr/local/src \
    && curl -fSL -o lua-intf-master.zip https://github.com/SteveKChiu/lua-intf/archive/master.zip \
    && unzip /usr/local/src/lua-intf-master.zip -d /usr/local/src \
    && curl -fSL -o LuaPbIntf-master.zip https://github.com/jinq0123/LuaPbIntf/archive/master.zip \
    && unzip /usr/local/src/LuaPbIntf-master.zip -d /usr/local/src

# Build Lua
#
RUN \
    cd /usr/local/src/lua-${LUA_VERSION} \
    && make linux install MYCFLAGS="-x c++" CC="g++" \
    && ldconfig

# Build Protocol Buffers
#
RUN \
    cd /usr/local/src/protobuf-${PROTOBUF_VERSION} \
    && ./autogen.sh \
    && ./configure CXXFLAGS="-fPIC" \
    && make -j${RESTY_J} \
    && make install \
    && ldconfig

# Build LuaPbIntf
#
RUN \
    cd /usr/local/src/LuaPbIntf-master \
    && mkdir -p /usr/local/src/LuaPbIntf-master/third_party/lua \
    && cp -r /usr/local/src/lua-${LUA_VERSION}/src /usr/local/src/LuaPbIntf-master/third_party/lua \
    && mkdir -p /usr/local/src/LuaPbIntf-master/third_party/protobuf \
    && cp -r /usr/local/src/protobuf-${PROTOBUF_VERSION}/src /usr/local/src/LuaPbIntf-master/third_party/protobuf \
    && mkdir -p /usr/local/src/LuaPbIntf-master/third_party/lua-intf \
    && cp -r /usr/local/src/lua-intf-master/LuaIntf /usr/local/src/LuaPbIntf-master/third_party/lua-intf \
    && cd /usr/local/src/LuaPbIntf-master/build \
    && sed -i 's!TARGETDIR = bin/Release!TARGETDIR = /usr/local/lib/lua/5.3!' LuaPbIntf.make \
    && make config=release

# Test require luapbintf
#
RUN /usr/local/bin/lua -l luapbintf

# Cleanup
#
RUN cd /usr/local/src \
    && rm -rf lua-${LUA_VERSION}.tar.gz \
    && rm -rf lua-${LUA_VERSION} \
    && rm -rf protobuf-cpp-${PROTOBUF_VERSION}.zip \
    && rm -rf protobuf-${PROTOBUF_VERSION} \
    && rm -rf lua-intf-master.zip \
    && rm -rf /usr/local/src/lua-intf-master \
    && rm -rf LuaPbIntf-master.zip

#
CMD [ "lua" ]
