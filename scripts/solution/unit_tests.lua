#!lua

--[[

  Creates solution that only contains Unit Tests

]]--


-- Run dependencies --
package.path = './?.lua;' .. package.path
dofile("./premake_data.lua")
dofile("./defaults.lua")


-- Solution settings --
solution_data = {
  name = "UnitTests",
}


-- Find the projects for the solution and add them --

-- Dependencies --
dofile("../../thirdparty/catch/data/catch.project")
dofile("../../thirdparty/remotery/data/remotery.project")

-- Projects to test --
dofile("../../lib/lib/data/lib.project")
dofile("../../lib/lib/data/lib.test_project")

dofile("../../lib/math/data/math.project")
dofile("../../lib/math/data/math.test_project")

dofile("../../lib/math/data/nil.project")
dofile("../../lib/math/data/nil.test_project")

dofile("../../lib/math/data/roa.project")
dofile("../../lib/math/data/roa.test_project")


-- Generates the premake api calls.
make.create_solution(
  solution_data,
  project_defaults,
  projects
)
