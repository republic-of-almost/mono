#!lua


--[[

  Creates a solution that contains every project and test project in the
  mono repo.

]]--


-- Run dependencies --
package.path = './?.lua;' .. package.path
dofile("./data_to_premake.lua")
dofile("./defaults.lua")


-- Solution Settings --
solution_data = {
  name = "RepublicOfAlmost",
}

-- Find the projects for the solution and add them --
project_files = os.matchfiles("../../**.json")

if project_files then

  --print("Loading project json")

  for i, proj in ipairs(project_files) do
    json_str = io.readfile(proj)
    proj_table, err = json.decode(json_str);

    if err then
      print("Failed to load json" .. err)
    else
      proj_table["base_location"] = path.getdirectory(proj) .. "/";

      table.insert(projects, proj_table)

      --print(proj_table["name"] .. " loaded");
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
