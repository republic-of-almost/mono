# Nil Readme

Nil is not meant to be a fancy API, it should be considered just the data layer for which
first and third parties can tap into, prototype on, and have an API on top.

Nil is a scene graph. Checkout Nil::Node for full details.
You can attach one of any supporting Nil::Data to a node. a node can children, you can walk
the node. Transforms are nested.


## Design Goals

- Should provide a robust scene graph.
- Low level access to data types `cameras[]`, `materials[]` etc.
- Look after data dependencies.
- POD data only (for data and resources).
- Flexibily and Speed over safty.


## Data and Resources

Most data has to be attached to a node for it to exist. The exceptions are Transforms that are added by default. This data is special cased because it is very common and has inherited properties.

Nil also has the concept of resources which are assets, data's who's lifetime is not attached to the lifetime of a node.


## Examples

How to examples.

### Setting Data

For the most part the application will work by getting/setting data on a node.

```cpp

Nil::Node node;
node.set_name("foo");

Nil::Data::Transform transform;
// set the transform.

Nil::Data::set(node, transform);

```

It will be up to the various sub-systems to translate this data for there own needs.
