# Nil Resource

Resources unlike data are not attached to nodes. Typically these represent assests.

## POD Types

All data types are pod types. So be sure to construct with uniform init if you are not setting all the data.

## Ownership Semantics

Unless stated any data passed into the engine will be copied out and owned. Thus calling data can be delete after its been passed into the engine.