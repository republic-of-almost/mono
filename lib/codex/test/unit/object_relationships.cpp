#include <codex/codex.h>
#include <catch/catch.hpp>
#include <vector>


TEST_CASE("Codex Relationships")
{
  codex_ctx *ctx = nullptr;
  codex_create(&ctx);

  SECTION("Set no parent")
  {
    const CODEX_BOOL fail_to_set_parent = codex_object_set_parent(ctx, 123, 321);
    REQUIRE(fail_to_set_parent == CODEX_FALSE);
  }

  SECTION("No children")
  {
    const unsigned parent = codex_object_create(ctx);

    unsigned childless = 0;
    codex_object_get_children(ctx, parent, nullptr, &childless);

    REQUIRE(childless == 0);
  }

  SECTION("Self parent")
  {
    const unsigned parent = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, parent, parent);
    REQUIRE(set_parent == CODEX_FALSE);
  }

  SECTION("Set parent")
  {
    const unsigned parent = codex_object_create(ctx);
    const unsigned child = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, child, parent);

    REQUIRE(set_parent == CODEX_TRUE);

    unsigned children = 0;
    codex_object_get_children(ctx, parent, nullptr, &children);

    REQUIRE(children == 1);

    std::vector<unsigned> child_ids(children);
    codex_object_get_children(ctx, parent, child_ids.data(), &children);

    REQUIRE(children == 1);
    REQUIRE(child_ids[0] == child);

    unsigned out_parent = 0;
    codex_object_get_parent(ctx, child_ids[0], &out_parent);

    REQUIRE(out_parent == parent);
  }

  SECTION("Unset parent")
  {
    const unsigned parent = codex_object_create(ctx);
    const unsigned child = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, child, parent);
    const CODEX_BOOL unset_parent = codex_object_set_parent(ctx, child, 0);

    REQUIRE(unset_parent == CODEX_TRUE);

    unsigned out_parent = 123;
    codex_object_get_parent(ctx, child, &out_parent);

    REQUIRE(out_parent == 0);
  }

  SECTION("destroy parent")
  {
    const unsigned parent = codex_object_create(ctx);
    const unsigned child = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, child, parent);
    const CODEX_BOOL destroy_parent = codex_object_destroy(ctx, parent);
  
    REQUIRE(destroy_parent == CODEX_TRUE);

    unsigned out_parent = 123;
    codex_object_get_parent(ctx, child, &out_parent);

    REQUIRE(out_parent == 0);
  }

  codex_destroy(&ctx);
}