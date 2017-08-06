#!lua

package.path = './?.lua;' .. package.path
dofile("./premake_data.lua")
dofile("./defaults.lua")


projects = {}

test_matches = os.matchfiles("../../**.test_project")

for i, proj in ipairs(test_matches) do
  dofile(proj)
end


-- Generates the premake code calls.
make.create_solution(
  solution_data,
  project_defaults,
  projects
)
