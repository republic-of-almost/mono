#!lua

package.path = './?.lua;' .. package.path
dofile("./premake_data.lua")


-- Solution data? Should this contain the the project names?
solution_data = {

  name = "RepublicOfAlmost",

}

-- Defaults will be based on platform in the future.
-- Currently only supports buildoptions.
project_defaults = {

  buildoptions_macosx = {
    "-std=c++14",
    "-stdlib=libc++",
  },

  buildoptions_windows = {
    "/IGNORE:C4577",
  },

  flags = {
    "EnableSSE2",
    "ExtraWarnings",
    "FloatFast",
    "NoExceptions", -- deprecated premake5
    "NoRTTI", -- deprecated premake5
  },

  defines = {
    "MATH_USE_SIMD",
  },

  defines_windows = {
    "_HAS_EXCEPTIONS=0",
    "_CRT_NO_WARNINGS",
  },

  exceptions = false,
  rtti = false,
}

-- This is used by the individual projects, but it needs to be defined first.
projects = {}

proj_matches = os.matchfiles("../../**.project")
test_matches = os.matchfiles("../../**.test_project")

for i, proj in ipairs(proj_matches) do
  dofile(proj)
end

for i, proj in ipairs(test_matches) do
  dofile(proj)
end

-- Generates the premake code calls.
make.create_solution(
  solution_data,
  project_defaults,
  projects
)
