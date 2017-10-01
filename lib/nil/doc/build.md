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

- **NIL_CPU_MEMORY_FIXED**
- **NIL_CPU_MEMORY_SCALE**
- **NIL_CPU_MEMORY_AUTO**

_Currently Unused - Need this for Emscripten_

Defines allocation statagy, Emscripten really needs fixed.

---

- **NIL_CPU_MEMORY_HINT=\<megabytes\>**

_Currently Unused - Helps define requirements and for Emscripten_

Define (in megabytes) a memory capacity hint, on Fixed platforms this hint will
not be broken.

---

- **NIL_CPU_TASK_COUNT=\<count\>**

_Currently Unused_

Some platforms have restrictions, web must have only one thread.
0 is Auto. Auto is defined as `n = (cores - 1)`

Platform                 | Settings
-------------------------|---------
Windows/Linux/Mac        | 0
Web                      | 1

---

- **NIL_GPU_TASK_MULTI_THREADED=\<0 or 1\>**

_Currently Unused_

If enabled one thread will be dedicated to the GPU tasks, else GPU tasks will
happen on any thread.


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

Preprocessor Flag                        | Default | Description
-----------------------------------------|---------|------------
NIL_MAX_TAG_NAME_LENGTH=\<count\>        | 64      | Max `char` length of a tag
NIL_MAX_NODE_NAME_LENGTH=\<count\>       | 32      | Max `char` length of a node name
NIL_GRAPH_TRANSFORM_STACK_HINT=\<count\> | 32      | Number of transforms to stack alloc on updates.
NIL_MAX_ASPECT_COUNT=\<count\>           | 16      | Max number of supported aspects.
NIL_MAX_CPU_TASK_COUNT=\<count\>         | 32      | Max CPU task per slot per tick.
NIL_MAX_GPU_TASK_COUNT=\<count\>         | 16      | Max GPU task per slot per tick.
NIL_CLEANUP_ON_EXIT=\<1 or 0\>           | 1       | If Nil should try and clean up on exit.
