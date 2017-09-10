# ROA Code Standards

General coding guidelines for ROA.

Nothing is sacrosanct!

## General

- Strong OO, scoped semantics etc.

## Classes that inherit from Object

- Shall have no members
- Shall do no data transforms

## Complex Classes

- Shall have no visible members other than pimpl.
- Follow RIIA for data cleanup.

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
