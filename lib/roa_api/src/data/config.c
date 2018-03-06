#include <data/config.h>
#include <roa_lib/hash.h>
#include <roa_lib/assert.h>


uint64_t hash_user;
uint64_t hash_job;
uint64_t hash_rendering;
uint64_t hash_physics;


void
rep_config_init()
{
  /* maybe we could hardcode these values instead */
  hash_user = roa_hash("user");
  hash_job = roa_hash("job");
  hash_rendering = roa_hash("rendering");
  hash_physics = roa_hash("physics");

  /* check hashses are unique - unlikely but we can afford to */
  ROA_ASSERT(hash_user != hash_job);
  ROA_ASSERT(hash_user != hash_rendering);
  ROA_ASSERT(hash_user != hash_physics);

  ROA_ASSERT(hash_job != hash_rendering);
  ROA_ASSERT(hash_job != hash_physics);

  ROA_ASSERT(hash_rendering != hash_physics);
}


uint64_t
rep_config_tagged_hash_user()
{
  return hash_user;
}


uint64_t
rep_config_tagged_hash_job()
{
  return hash_job;
}


uint64_t
rep_config_tagged_hash_rendering()
{
  return hash_rendering;
}


uint64_t
rep_config_tagged_hash_physics()
{
  return hash_physics;
}