# Nil

Nil is a combination of the data that represents the application and a scene graph.


## Data

Most data has to be attached to a node for it to exist. The exceptions are Transforms and bounding boxes that are added by default. This data is special cased because both are quite often used anyway and both have inherited properties.


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
