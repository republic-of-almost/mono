#include <codex/codex.h>
#include <catch/catch.hpp>
#include <roalib/array.h>


TEST_CASE("Codex Relationships")
{
  codex_ctx *ctx = nullptr;
  codex_create(&ctx);

  SECTION("Set no parent")
  {
    const CODEX_BOOL fail_to_set_parent = codex_object_set_parent(ctx, 123, 321);
    REQUIRE(fail_to_set_parent == CODEX_FALSE);
  }

  SECTION("Self parent")
  {
    unsigned parent = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, parent, parent);
    REQUIRE(set_parent == CODEX_FALSE);
  }

  SECTION("Set parent")
  {
    unsigned parent = codex_object_create(ctx);
    unsigned child = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, child, parent);
    REQUIRE(set_parent == CODEX_TRUE);
  }

  SECTION("Unset parent")
  {
    unsigned parent = codex_object_create(ctx);
    unsigned child = codex_object_create(ctx);

    const CODEX_BOOL set_parent = codex_object_set_parent(ctx, child, parent);
    const CODEX_BOOL unset_parent = codex_object_set_parent(ctx, child, 0);

    REQUIRE(unset_parent == CODEX_TRUE);
  }

  codex_destroy(&ctx);
}