# Nil Readme

Nil is a combination of the data that represents the application and a scene graph.


## Design Goals

The goal of Nil is to provide a graph which data can be attached, and a mechanism to allow other libraries to deal with resources and object lifetime.

Other than controlling callbacks and the graph. Nil doesn't actually do any work.

Nil is _meant_ to be low level / low user protection interface this is not designed to be a public facing interface. The one exception is Nil::Node nodes have complex lifetimes so Node is designed to be safe for public use.


## Data and Resources

Most data has to be attached to a node for it to exist. The exceptions are Transforms that are added by default. This data is special cased because it is very common and has inherited properties.

Nil also has the concept of resources which are assets, data's who's lifetime is not attached to the lifetime of a node.


## Future Things and TODO

  - Nice to move out transforms from the graph, and have graph just nodes, and transforms as data like the rest of them.


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
