# Nil Code Standards


General coding guidelines for Nil.

Nothing is sacrosanct!


## Main Public Engine/Graph Class Types

Follow RIIA for data cleanup.


## Resources and Data Types

These types must be POD types. All pointers should be considered owned by the code that alloced the data.
So any pointers loaded into a Resource or Data type will be internally copied.


## Folder Structure

- Use them!
- `/include/Nil` for public interface
- `/src/interface` for public interface src


## Allocation

You allocate you destroy it. You didn't allocate don't destroy it.


## Files

- All doc should be markdown .md
- All C++ headers should be .hpp
- All C++ src files should be .cpp
- All filse lowercase with unserscore for spaces
- Namespace and Types start with Caps and underscore for spaces
- Functions / methods lower case with underscore for spaces
