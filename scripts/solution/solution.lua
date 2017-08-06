#!lua


--[[

  Creates a solution that contains every project and test project in the
  mono repo.

]]--


-- Run dependencies --
package.path = './?.lua;' .. package.path
dofile("./premake_data.lua")
dofile("./defaults.lua")


-- Solution Settings --
solution_data = {
  name = "RepublicOfAlmost",
}


-- Find the projects for the solution and add them --
proj_matches = os.matchfiles("../../**.project")
test_matches = os.matchfiles("../../**.test_project")

for i, proj in ipairs(proj_matches) do
  dofile(proj)
end

for i, proj in ipairs(test_matches) do
  dofile(proj)
end


-- Generates the premake api calls --
make.create_solution(
  solution_data,
  project_defaults,
  projects
)
