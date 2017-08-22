# Nil Interface

Nil is not meant to be a fancy API, it should be considered just the data layer for which
first and third parties can tap into, prototype on, and if an API on top.

## Design Goals

- Should provide a robust scene graph.
- Low level access to data types `cameras[]`, `materials[]` etc.
- Look after data dependencies.
- POD data only (for data and resources).
- Flexibily and Speed over safty.

## Node

Nil is a scene graph. Checkout Nil::Node for full details.
You can attach one of any supporting Nil::Data to a node. a node can children, you can walk
the node. Transforms are nested.

_example_

```cpp
Nil::Node node;

// Set the node's data //
Nil::Data::Renderable data{};
Nil::Data::set(node, data);

// ... Later get the renderable //

Nil::Data::get(node, data);
```

Nodes are owned when you have the top level, if you parent a node then it becomes a reference.

_example_

```cpp
Nil::Node node; // Is owned. it will be deleted if it goes out of scope.

node.set_parent(other_node); // Node is now a reference.
```


## Data And Resources

Data is attached to a node, resources can be considered global to the application, and are
referenced by data via ids. All data and resources are POD types. Which means you should be using
using uniform constructor syntax `{}` or `memset` your variables.


## Ownership For Data And Resources

For Nil::Data and Nil::Resources ownership any pointer types will be copied over internally.
this means the calling code is safe to use stack objects and/or delete their heap objects.

_example_

```cpp
char name[512]{};
strcat(name, "foo");

Nil::Resources::Material mat{};
mat.name = name; // this will be copied internally, name is free to go out of scope.
```

## TODO
- Add to unit test CI.
- More Node tests for life time.
- More Node Data/Resource tests for life time.