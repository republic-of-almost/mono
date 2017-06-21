# ROV Impl

ROV has a data organisation part and an API implimentation part.

Essentially the data org will try and cull state changes to allow it to be batched
to the GPU.

The API will take that organised data and process it.
This could be tuned per API but the size of my projects its not worth it right now.
