# Nil Aspects Build

Build settings for the Aspects Controllers.

## Preprocessor Flags

**NIMGUI**

_Set in build configs in premake.lua_

Set this in the project file to disable all Imgui (and related) functionality.
This is good for profiling etc.

Config      | Set
------------|---------
Development | NO
Staging     | NO
Release     | YES

---

**NDEBUGLINES**

_Set in build configs in premake.lua_

Set this in the project file to disable all debug lines. Debug lines aren't
designed to be fast so this could give some perf.

Config      | Set
------------|---------
Development | NO
Staging     | NO
Release     | YES

---

**NVRSUPPORT**

_Experimental_

Set this in the project file to disable VR support. This means the project doesn't have to link to the 3rdparty and that the GPU resources aren't allocated for the eyes.
