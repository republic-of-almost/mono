# ROA Todo's


### Nil::Node / ROA::Object

These two objects are the same...


### Remove Logic Class

Superseded by component class.


### Provide a better way of getting to nodes.

Right now we have to loop over everything to find the node we want.
better if we could provide obj.find("name/name/name");


### Provide range based looping for objects

They are very comment, an ROA::Object_collection would be handy to allow

for(auto &obj : other_obj.get_children())
{

}


### Transform

Sort out local / world.


### Object Copying

This fails, it shouldn't. The underlying object gets destroyed.

```
ROA::Inerherited_from_obj obj;
obj = some_obj->get_inheritecd_obj()
```
