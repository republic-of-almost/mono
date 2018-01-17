#include <codex/codex.h>
#include <catch/catch.hpp>

/*
unsigned obj_id = codex_create_object();
codex_object_set_parent(obj_id, other_obj_id);


codex_writer *writer = codex_writer(id);
codex_writer_commit(writer);


float position[3];
codex_read_float(id, L_POSITION_XYZ, position);
codex_read_float(id, L_SCALE_XYZ, data);

*/

TEST_CASE("Codex Object")
{
  codex_ctx *ctx = nullptr;
  codex_create(&ctx);

  SECTION("Create/Destroy")
  {
    const CODEX_BOOL should_not_exist = codex_object_exists(ctx, 123);
    REQUIRE(should_not_exist == CODEX_FALSE);

    const unsigned obj_id = codex_object_create(ctx);
    REQUIRE(obj_id != 0);
    REQUIRE(codex_object_count(ctx) == 1);

    const CODEX_BOOL should_exist = codex_object_exists(ctx, obj_id);
    REQUIRE(should_exist == CODEX_TRUE);

    const int destroy_real_id = codex_object_destroy(ctx, obj_id);
    REQUIRE(destroy_real_id == CODEX_TRUE);
    REQUIRE(codex_object_count(ctx) == 0);

    const int destroy_unknown_id = codex_object_destroy(ctx, 123);
    REQUIRE(destroy_unknown_id == CODEX_FALSE);
    REQUIRE(codex_object_count(ctx) == 0);
  }

  SECTION("Callbacks")
  {
    int created_counter = 0;
    auto created = [](unsigned obj_id[], unsigned count, void *user_data)
    {
      REQUIRE(obj_id != nullptr);
      REQUIRE(obj_id[0] == 1);
      REQUIRE(count == 1);
      ++(*(int*)user_data);
    };

    int destroyed_counter = 0;
    auto destroyed = [](unsigned obj_id[], unsigned count, void *user_data)
    {
      REQUIRE(obj_id != nullptr);
      REQUIRE(obj_id[0] == 1);
      REQUIRE(count == 1);
      ++(*(int*)user_data);
    };

    codex_callbacks cbs {
      created,
      (void*)&created_counter,

      destroyed,
      (void*)&destroyed_counter,
    };

    codex_object_callbacks(ctx, &cbs);

    unsigned obj_id = codex_object_create(ctx);
    codex_object_destroy(ctx, obj_id);

    REQUIRE(created_counter == 1);
    REQUIRE(destroyed_counter == 1);
  }

  codex_destroy(&ctx);
}