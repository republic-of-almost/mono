# Code Standards

Each project defines its standards based on its own constraints. These are solution wide rules of thumbs.


## Rules of Thumb

The rule of thumb is use what you _need_, ask yourself does this language feature make the code _better_?

The following is a set of guidelines that each project should consider. Each project will have its own stresses and requirements so each project will define its own set of rules.


### Encouraged

  - No / Simple Inheritance - _Less programmer overhead_
  - POD Types - _Simpler mechanics_
  - Data over Code - _Easier to maintain_
  - Stack allocations - _Performance_
  - Clear ownership semantics - _Programmer overhead_
  - Define all ctors/dtors for classes - _Helps debugging_
  - 80 Character width - _Existing tools, Terminal etc_
  - tabs set to spaces, and tab length set to 2 - _Suggestion_
  - Pass of allocations to calling code - _Performance_
  - Leave code tidier than you found it - _Agile can accumulate debt_
  - Write code that is easily optimized - _Data!_
  - Follow existing code style for a file/library - _Throw the ring into Mordor_


### Discouraged

  - Shared ownership - _Programmer overhead_
  - Complex inheritance - _Programmer overhead_
  - Heap allocations - _Performance_
  - Classes with there own 'environments' - _Hard to optimize_
  - STL / Templates - _Slower builds, defined in headers, etc_
  - Exceptions / GOTOs - _Goto's are hard to follow, exceptions little use_
  - RTTI / String Compare - _Performance_
  - Treating the next coder that comes along like a moron - _Closer to the fire_
