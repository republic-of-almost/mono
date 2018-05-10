

```
struct mesh_rsrc
{
    /* arr */ mesh_desc *pending;

    /* arr */ ids;
    /* arr */ mesh_data;
    /* arr */ culling_sphere;
};


struct texture_rsrc
{
    /* arr */ texture_desc *pending;

    /* arr */ ids;
    /* arr */ texture_data;
};


struct material_rsrc
{
    /* arr */ ids;
    /* arr */ material_data;
};


struct draw_call
{
    material_id;
    mesh_id;
}


struct renderables
{
    /* arr */ ids;
    /* arr */ materials;
    /* arr */ mesh;
    /* arr */ world_transforms;
    /* arr */ culling_spheres;
}

for(cameras)
{
    camera_drawables[];

    cam_sphere;
    for(culling_spheres)
    {
        if(inside)
        {
            camera_drawables.push(id);
        }
    }

    camera_drawcalls;

    for(camera_drawables)
    {
        draw_calls.push(world_mat | mesh | material);
    }

    for(transforms)
    {
        world_view_proj_mat;
    }
}

```
