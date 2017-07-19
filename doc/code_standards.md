# Code Standards

Each project defines its standards based on its own constraints. These are solution wide rules of thumbs.


## Rule of Thumbs

The rule of thumb is use what you _need_, ask yourself does using this make the code _better_?


### Encouraged

  - No / Simple Inheritance.
  - POD Types.
  - Data over Code.
  - Stack allocations.
  - Clear ownership semantics.
  - Defined ctors/dtors even if empty for classes.
  - 80 Character width.
  - tabs set to spaces, and tab length set to 2.
  - Pass of allocations to calling code.

### Discouraged

  - Shared ownership.
  - Complex inheritance.
  - Heap allocations.
  - Classes with there own 'environments'.
  - Templates.
  - STL.
  - Exceptions / Gotos.
  - RTTI / String Compares
  - Treating the next coder that comes along like a moron.
