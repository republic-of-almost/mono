# Nil Data

These types of data can be attached to nodes.

## Desgin Goals

- Resources should be POD types.
- Pointers are owned by the origin of the data.
  - ie. Resource ptrs created by calling code is owned by calling code.
  - ie. Resource ptrs fetched by a find method is owned by Nil.
- The data is owned by the node, if the node is destroyed the data is as well.

## TODO
- Be able to delete data without destroing the node.
- Remove global initialization, initialize data at Nil::Engine CTOR time.