# ROV Build

Build settings for ROV

## Preprocessor Flags

- **ROV_NOP**
- **ROV_GL4**

Build with all the api flags you wish to support. You can have multiple backends
compiled in, you have to select what graphics API you wish at runtime.

Nop is for testing only it renders nothing. This is used for ROV unit tests,
and can be used for testing other aspects of the engine that require a renderer.

---

**ROV_GL4_VR_PROTO**

Enable the VR prototype. This is hackish, it may or may not work.
