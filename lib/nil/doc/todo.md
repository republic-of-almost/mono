# Nil Todo's


### Delete Data

You should be able to delete data without having to destroy the node.

### Memory

Nil should be able to hand out memory both CPU and GPU, GPU is complicated but
I think this meakes better sense.


### Delete Resources

We should be able to delte a resource.


### C linkage interface

Consider would `transform_set(node, transoform)` be better than\
`set(node, transform)`?


### Nil::Node / ROA::Object

These two objects are the same, can we have nil expose a lower level graph node?
Internally Nil doesn't use Nil::Nodes so could we just have a POD type entity?


### Resource Lifetime

We need to be sure this is working.


### Transforms / Other Nested Data

Transforms should be treated more like other data and have the graph only deal
with relationships.

This would allow us to add other inherited data like BHV's for example.


### Last Used Resource

Last used on resources would allow us to be able make better choices later down
the line. Ie dump a high res texture in favour of a low res one etc.


### Hash Strings

The string lookup for names and stuff is bad and should be removed.


### Move A Resource

We should be able to avoid excessive copying of textures like we do.


### Data to Resources

Somethings in Data should be resources, gamepad/keyboard etc.


### Move Tags out of Node

Doing this will allow us to be able to query globally the tags used.


### Some default Resources and Data

Would be handy to have a 'No Texture' texture so when something goes wrong we
can see a texture that says 'No Texture' on it.

Replicating this across other resources would be good. A cross mesh etc.
