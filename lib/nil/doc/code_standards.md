# Nil Code Standards

General coding guidelines for Nil

## Main Public Engine/Graph Class Types

Follow RIIA, for data cleanup.

## Resources and Data Types

These types must be POD types. All pointers should be considered owned by the code that alloced the data.
So any pointers loaded into a Resource or Data type will be internally copied.

## Folder Structure

- Use them!
- /include/Nil for public interface
- /src/interface for public interface src

## Allocation

You allocate you destroy it. You didn't allocate don't destroy it.

## General

- No STL in public interface / can be used as required in src files
- Fixed integer only (unless dealing with 3rdparty)
- No doubles (unless dealing with 3rdparty)
- Use fwd dec files
- Leave a file in better shape than you found it
- Follow a files style or refresh it to a new style, no half style
- Branches must have braces, no restriction on where branch braces go
- Favour branch on new line, smaller can be inline
- Favour 80 character width limit
- favour int comparisons over string ones hash if it helps
- No RTTI
- No Exceptions
- No Inheritance

## Files

- All doc should be markdown .md
- All C++ headers should be .hpp
- All C++ src files should be .cpp
- All filse lowercase with unserscore for spaces
- Namespace and Types start with Caps and underscore for spaces
- Functions / methods lower case with underscore for spaces
