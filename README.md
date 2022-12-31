# chunk

## parallel HTTP/1.1 bytes-on-wire-to-ext4 transmuter

### usage
```commandline
chunk 127.0.0.1 80 index.html
```

### quick start
```commandline
git clone https://github.com/gnu-enjoyer/chunk
cd chunk
cmake -B build && cmake --build build
```

minimal requirements beyond a modern C++ compiler and CMake; or treat yourself to the provided `Dockerfile`

### misc

no DNS support due to glibc NSS misbehaving when statically linked