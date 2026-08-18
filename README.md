# Antithesis Stub Library

This is the source of `libvoidstar.so`, a native library that the 
[Antithesis](https://antithesis.com) SDKs use to provide part of their 
functionality, and that software using LLVM coverage instrumentation 
can link against.

**ORDINARY USERS OF ANTITHESIS DO NOT NEED TO THINK ABOUT THIS LIBRARY**
**IT IS ONLY RELEVANT TO PEOPLE USING THE LEGACY C/C++ AND RUST SDKS**
**OR FOR PEOPLE WHO ARE IMPLEMENTING THEIR OWN THIRD PARTY SDKS.**

Inside Antithesis, the `libvoidstar.so` name resolves to a library
which serves as the runtime interface between your program and the
Antithesis environment. Outside Antithesis — in your CI, on your laptop —
it resolves to this stub, so the same binary runs unmodified anywhere:

- `fuzz_get_random` returns real entropy from the kernel, so SDK
  randomness keeps working;
- all output functions are no-ops;
- LLVM coverage guards are left zeroed, so instrumented call sites
  (`if (*guard) ...`) never call into the library at all, and your
  program runs at close to native speed.

## Building

There is deliberately no build system. It is one self-contained C file;
build it with whatever toolchain you already use:

```sh
cc -O2 -shared -fPIC -o libvoidstar.so libvoidstar.c
```

If you would rather not build it yourself, Antithesis distributes a
prebuilt binary in the [Releases](https://github.com/antithesishq/antithesis_stub/releases) section. The prebuilt binary 
is linked against glibc; if you build with musl, compile this file 
with your own toolchain instead.

Do not link this library statically, since that defeats its entire purpose. If 
your program must be fully static, contact Antithesis support so we can help 
you.

## Compatibility

The exported symbols and their behavior are an ABI that the Antithesis
platform maintains: binaries linked against this stub work when the real
library is substituted at runtime, and vice versa.

The file deliberately uses nothing newer than glibc-2.2.5, so it compiles
and runs on any Linux from this century. 

Consumers of the SDK randomness functions require Linux 3.17 or later.

## License

[MIT](LICENSE)
