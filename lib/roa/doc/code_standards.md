# ROA Code Standards

The design goal of ROA is to provide an clean interface for the engine.

## Code Standards

- Favor OO for node data.
- Favor namespaced functions for global resources.
- Favor using final on objects.
- Classes that inherit from Object should have no member variables.


## Code Style

### Header files

```cpp

namespace ROA { // inline ns


class Someclass final
{
  void        inlined_method_names();
  void        for_each_method();
};


void      also_inline_functions();
void      in_namespace();


};

```
