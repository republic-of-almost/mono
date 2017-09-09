# Nil Build

Details relating to building Nil.

## Preprocessor Flags

- **NIL_DEVELOPMENT (Development Config)**
- **NIL_STAGE (Staging Config)**
- **NIL_RELEASE (Release Config)**

_Set in build configs in premake.lua_
Config      | Settings
------------|---------
Development | symbols, no optimisations
Staging     | symbols, optimisations, ~~warnings as errors~~ (in future)
Release     | no symbols, optimisations, ~~warnings as errors~~ (in future)

---

- **NIL_MEMORY_FIXED**
- **NIL_MEMORY_SCALE**
- **NIL_MEMORY_AUTO**

_Currently Unused - Need this for Emscripten_

Defines allocation statagy, Emscripten really needs fixed.

---

- **NIL_MEMORY_HINT**

_Currently Unused - Helps define requirements and for Emscripten_

Define (in megabytes) a memory capacity hint, on Fixed platforms this hint will
not be broken.

---

- **NIL_PEDANTIC**

_Set in build configs in premake.lua_

More logging, less parameter checking.

Config        | Enabled
--------------|---------
Development   | TRUE
Staging       | TRUE
Release       | FALSE


---

**General Settings**

_Place these in the `nil.project defines { ... }` to alter the defaults._

Things that can be

Preprocessor Flag              | Default | Description
-------------------------------|---------|------------
NIL_MAX_TAG_NAME_LENGTH        | 64      | Max `char` length of a tag
NIL_MAX_NODE_NAME_LENGTH       | 32      | Max `char` length of a node name
NIL_GRAPH_TRANSFORM_STACK_HINT | 32      | Number of transforms to stack alloc on updates.
