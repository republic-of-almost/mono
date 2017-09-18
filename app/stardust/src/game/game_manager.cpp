#include <game/game_manager.hpp>
#include <game/actor_kinematic.hpp>
#include <lib/assert.hpp>
#include <string.h>


namespace Game {


void
Game_manager::on_start()
{
  // -- This -- //
  {
    ROA::Object this_obj = get_object();
    this_obj.set_name("Game Manager");
  }
  
  // -- Load Scene -- //
  constexpr float scene_scale = 4.f;
  {
    scene = ROA::Object{};

    scene.set_name("Game Scene");
    ROA::Scene::load(scene, "mesh/test_level.gltf");
    
    ROA::Transform trans = scene.get_transform();
    
    
    trans.set_scale(ROA::Vector3(scene_scale, scene_scale, scene_scale));
  }
  
  // -- Find Spawn Point -- //
  /*
    This currently fails, for a bunch of reasons see todo's.
  */
//  ROA::Object spawn_point;
//  {
//    const size_t children = scene.get_child_count();
//    
//    for(size_t i = 0; i < children; ++i)
//    {
//      ROA::Object child = scene.get_child(i);
//      const char *child_name = child.get_name();
//      
//      if(strcmp(child_name, "Warehouse") == 0)
//      {
//        const size_t gran_children = child.get_child_count();
//        
//        for(size_t j = 0; j < gran_children; ++j)
//        {
//          ROA::Object gran_child = child.get_child(j);
//          const char *gran_child_name = gran_child.get_name();
//        
//          if(strcmp(gran_child_name, "SpawnPoint") == 0)
//          {
//            spawn_point = gran_child;
//            break;
//          }
//        }
//      }
//    }
//  }

  // -- Create the Actor -- //
  {
    actor = ROA::Object{};
    actor.add_component<Actor_kinematic>();
    actor.set_name("Actor");
    
    Actor_kinematic *actor_comp = actor.get_component<Actor_kinematic>();
    LIB_ASSERT(actor_comp);
    
    // -- Find Navmesh -- //
    {
      ROA::Mesh mesh = ROA::Mesh("navmesh");
      
      if(mesh.is_valid())
      {
        float *verts = mesh.get_positions_vec3();
        uint32_t *index = mesh.get_index();
        
        const size_t tri_count = mesh.get_triangle_count();
        const size_t index_count = mesh.get_index_count();
        
        float *nav_mesh = (float*)malloc(sizeof(float) * (index_count) * 3);
        
        for(size_t i = 0; i < index_count; ++i)
        {
          const size_t nav_index = i * 3;
          const size_t vert_index = index[i] * 3;
          
          LIB_ASSERT(nav_index / 3 < index_count);
          
          nav_mesh[nav_index + 0] = verts[vert_index + 0] * scene_scale;
          nav_mesh[nav_index + 1] = verts[vert_index + 1] * scene_scale;
          nav_mesh[nav_index + 2] = verts[vert_index + 2] * scene_scale;
        }
        
        actor_comp->nav_mesh = nav_mesh;
        actor_comp->nav_mesh_count = tri_count;
      }
    }
  }
  
}


void
Game_manager::on_think()
{
  // -- Game Conditions -- //
}


} // ns
