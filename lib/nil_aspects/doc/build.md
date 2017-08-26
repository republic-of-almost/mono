# Nil Aspects Build

Build settings for the Aspects Controllers.

## Preprocessor Flags

**NIMGUI**
Set this in the project file to disable all Imgui (and related) functionality.
This is good for profiling etc.

**NDEBUGLINES**
Set this in the project file to disable all debug lines. Debug lines aren't
designed to be fast so this could give some perf.

**NVRSUPPORT**
Set this in the project file to disable VR support. This means the project doesn't have to link to the 3rdparty and that the GPU resources aren't allocated for the eyes.
