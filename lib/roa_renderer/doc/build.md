# ROA Renderer Build

## Graphics API Flag

One of these flags should be defined

**ROA_RENDERER_API_NOOP**

One `ROA_RENDERER_API_` flag should be defined.

This API is for testing, where the front end of the renderer will still execute but no renderpasses are processed.

This is primiarly for unit/functional testing where the host may not have a graphics API.

**ROA_RENDERER_API_GL4**

One `ROA_RENDERER_API_` flag should be defined.

This flag is for OpenGL 4.x
