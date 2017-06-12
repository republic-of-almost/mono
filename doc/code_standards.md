# Code Standards

Each project defines its standards based on its own constraints. These are only
rules of thumbs.

## Rule of Thumbs

The rule of thumb is use what you _need_.

### Encouraged

  - No / Simple Inheritance.
  - POD Types.
  - Data over Code.
  - Stack allocations.
  - Clear ownership semantics.
  - Defined ctors/dtors even if empty.

### Discouraged

  - Shared ownership.
  - Complex inheritance.
  - Heap allocations.
  - Classes with there own 'environments'.
  - Templates.
  - STL.
  - Exceptions / Gotos.
