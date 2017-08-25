# Render-O-Vision

ROV is to be a _lightweight_ mesh renderer. I may split this project up later
to have Hardware resources handled by a different library. That depends on if
I want to have post processes and particles handled here or not.

## Design Goals

A submission style renderer, for example ...

```cpp
set_mesh();
set_camera();
submit();

execute();
```

The submission can happen on any thread but the execution must happen on the
gpu thread.

Currently ROV has no requirements to allow destroying of resources at runtime.
The games it was intended for are simple, so we can say for now that resources
are for the lifetime of the application.

ROV currently supports a debug line renderer.

## Future Considerations

ROV might include _post_ filters later.
ROV might include some _heuristics_ front to back.
ROV might deal with _particles_.

## TODO

- Can we remove the VAO rebinds?
- Can we batch create the textures? We are already queuing up the textures!
- Can we batch create the meshes? We are already queuing up the textures!
- FBO's
- Other API's
