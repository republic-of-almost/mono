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
--proj_matches = os.matchfiles("../../**.project")
--test_matches = os.matchfiles("../../**.test_project")
project_files = os.matchfiles("../../**.json")

-- for i, proj in ipairs(proj_matches) do
--   dofile(proj)
-- end

-- for i, proj in ipairs(test_matches) do
--   dofile(proj)
-- end


if project_files then

  print("Loading project json")

  for i, proj in ipairs(project_files) do
    json_str = io.readfile(proj)
    proj_table, err = json.decode(json_str);

    if err then
      print("Failed to load json" .. err)
    else
      proj_table["base_location"] = path.getdirectory(proj);

      table.insert(projects, proj_table)

      print(proj_table["name"] .. " loaded");
    end
  end

  -- Generates the premake api calls --
  make.create_solution(
    solution_data,
    project_defaults,
    projects
  )


else

  print("No data to load")

end