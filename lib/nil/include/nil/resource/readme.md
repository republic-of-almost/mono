# Nil Resource

Resources unlike data are not attached to nodes. Typically these represent assests.

## Desgin Goals

- Resources should be POD types.
- Pointers are owned by the origin of the data.
  - ie. Resource ptrs created by calling code is owned by calling code.
  - ie. Resource ptrs fetched by a find method is owned by Nil.

## TODO
- Be able to delete resources.
- Last used on resources.
- Remove global initialization, initialize resources at Nil::Engine CTOR time.
- Hash names for lookup, string lookup bad!