### Build if you require

```
$ docker build -t vkill/luapbintf:xenial --build-arg RESTY_J=4 ./xenial
```

### Usage

```
$ docker run -it --rm --name luapbintf_xenial vkill/luapbintf:xenial

# Lua 5.3.4  Copyright (C) 1994-2017 Lua.org, PUC-Rio
# > require 'luapbintf'
# table: 0x1b73010
# > os.exit(0)
```

$ docker run -it --rm --name luapbintf_xenial -v $(pwd)/..:/tmp/luapbintf vkill/luapbintf:xenial lua /tmp/luapbintf/test/test.lua
```
