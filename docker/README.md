### Build

```
$ docker build -t vkill/luapbintf:xenial --build-arg RESTY_J=4 ./xenial
```

### Test

```
$ docker run -it --rm --name luapbintf_xenial -v $(pwd)/..:/tmp/luapbintf vkill/luapbintf:xenial lua /tmp/luapbintf/test/test.lua
```
