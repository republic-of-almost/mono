# ROA

Experimental API to expose a 'clean' interface to Nil.


## Design Goals


### Overview

A place to hide implimentation details to keep them out of Nil. The hope is we can keep Nil very specific to engine development and have the API sugar hidden in ROA.

There is an argument to move Nil::Node to here, and just have Nil use uint32_t's as it internall does already.


### Public Headers

More so than normal, but headers should contain as minimal includes as possible. We don't want to pollute the users code base.


## Test

Unsure what the tests should be here. In theory this should mostly be exposing or wrapping existing functionality so for now no test stratagy exists.


## Danger

There is a danger this will just act as a mirror of Nil.
