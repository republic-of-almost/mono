# Math Code Standards

General coding guidelines for Nil.
Nothing is sacrosanct!


## Free Functions

Use free functions for all math, compilers are better and dealing with optimizing
this.


## Inline

All functions should be inline, and should be easily tweaked to force inline.
This should be done per math module.


## Data

Provide as many basic data entry points as reasonable, Dealing with 3rdparty
and hardware requires underlying type access, don't hide. it.


## Includes


This library should be very portable, use #include "" instead of #include <>


## Dependencies

This library should be very portable, Math should depend on nothing other than
platform includes.
