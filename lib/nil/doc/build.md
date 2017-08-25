# Nil Build

## Preprocessor Flags

**NIL_DEVELOPMENT (Development Config)**
**NIL_STAGE (Staging Config)**
**NIL_RELEASE (Release Config)**
These are set in premake data, each defines the IDE/Makefile config.
Development = symbols, no optimisations.
Staging = symbols, optimisations.
Release = no symbols, optimisations.

**NIL_MEMORY_FIXED**
**NIL_MEMORY_SCALE**
**NIL_MEMORY_AUTO**
Build with one these to define the memory characteristics. Fixed is preferable,
and a requirement on some platforms.

**NIL_MEMORY_HINT**
Define (in megabytes) a memory capacity hint, on Fixed platforms this hint will
not be broken.
