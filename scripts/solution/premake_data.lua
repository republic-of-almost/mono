#!lua

make = {} -- namespace


----------------------------------------------------------------- [ HELPERS ] --


-- Gets the root of the file.
function
make.get_proj_root()
  local str = debug.getinfo(2, "S").source:sub(2)
  return str:match("(.*/)")
end


-- Adds source file patterns
function
make.add_src(dir)
  return {
    dir .. "**.cpp",
    dir .. "**.cc",
    dir .. "**.mm",
    dir .. "**.c",
    dir .. "**.m",
  }
end


-- Adds header file patterns
function
make.add_headers(dir)
  return {
    dir .. "**.hpp",
    dir .. "**.hh",
    dir .. "**.h",
  }
end


-- Adds doc file patterns
function
make.add_doc(dir)
  return {
    dir .. "**.txt",
    dir .. "**.md",
  }
end


------------------------------------------------------ [ SOLUTION GENERATOR ] --


-- Generates a solution.
function
make.create_solution(solution_data, project_defaults, projects)

  -- Create the solution for the project.
  print("Creating Solution")

  solution(solution_data.name)
  location("./")

  configurations({
    "Debug",
    "Release"
  })

  -- Create the projects
  -- for i, proj in ipairs(arg) do -- old way.

  for i, proj in ipairs(projects) do

    print("Building project: " .. proj.name)

    -- Fill in the default information.
    if not proj.kind                  then proj.kind = "ConsoleApp"        end
    if not proj.lib_directories       then proj.lib_directories = {""}     end
    if not proj.include_directories   then proj.include_directories = {""} end

    -- Generate the project data.
    project(proj.name)
    location(proj.location)
    language(proj.language)
    kind(proj.kind)

    if proj.uuid then uuid(proj.uuid) end

    -- Thie function takes a string that represents a field
    -- to search in the table. it will then append the premakes
    -- platform name to the end, and search for that field.
    function
    find_table_with_platform(proj, string)
      local result_table = {}
      local call_str = "result_table = proj." .. string .. "_" .. os.get()

      -- Need to watch this - loadstring is depreated, but premake's version of lua is old.
      local chunk = loadstring(call_str)
      setfenv( chunk, { result_table = result_table, proj = proj } )
      chunk()

      -- This is new method, should premake's lua version change.
      --load(call_str, nil, nil, my_env)()
      --local my_env = { table = table, proj = proj}

      return getfenv(chunk).result_table
    end

    -- Preprocessor
    if proj.defines then defines(proj.defines); end

    local platform_defines = find_table_with_platform(proj, "defines")
    if platform_defines then defines(platform_defines) end

    -- Src files
    if proj.src_files then files(proj.src_files) end

    local platform_src = find_table_with_platform(proj, "src_files")
    if platform_src then files(platform_src) end

    -- Excludes
    if proj.src_excludes then excludes(proj.src_excludes) end

    local platform_exclude = find_table_with_platform(proj, "src_files_exclude")
    if platform_exclude then excludes(platform_exclude) end

    -- Include dirs
    if proj.inc_dirs then includedirs(proj.inc_dirs) end

    local platform_inc_dirs = find_table_with_platform(proj, "inc_dirs")
    if platform_inc_dirs then includedirs(platform_inc_dirs) end

    -- Library directories
    if proj.lib_dirs then libdirs(proj.lib_dirs) end

    local platform_lib_dirs = find_table_with_platform(proj, "lib_dirs")
    if platform_lib_dirs then libdirs(platform_lib_dirs) end

    -- Link options
    if proj.linkoptions then  linkoptions(proj.linkoptions) end

    local platform_link_options = find_table_with_platform(proj, "linkoptions")
    if platform_link_options then linkoptions(platform_link_options) end

    -- Links and Link dependencies
    if proj.links then links(proj.links) end

    -- If we have project dependencies then we need
    -- to check if that project specifies any links
    -- as dependencies.
    if proj.project_dependencies then

      -- Loop through each of the dependencies
      for i, dep in ipairs(proj.project_dependencies) do

        -- Loop through the projects we have been given.
        for j, other_proj in ipairs(projects) do

          -- If a match then check for links
          if dep == other_proj.name then

            print("  -- Adding Dep: " .. dep);

            -- Projects can be marked no link
            -- But still want to bring in header files etc.

            link = true;
            if other_proj.no_link == true then link = false end -- deprecated
            if other_proj.lang_settings then
              if other_proj.lang_settings.no_link == true then link = false end
            end

            if link then

              -- Add this project as a dep
              links(other_proj.name)

              -- Add listed project deps
              if other_proj.link_dependencies then links(other_proj.link_dependencies) end

              -- Find platform deps
              local platform_dep_links = find_table_with_platform(other_proj, "link_dependencies")
              if platform_dep_links then links(platform_dep_links) end

              -- Add link option dependencies
              if other_proj.linkoption_dependencies then linkoptions(other_proj.linkoption_dependencies) end

              -- Platform
              local platform_dep_link_options = find_table_with_platform(other_proj, "linkoption_dependencies")
              if platform_dep_link_options then linkoptions(platform_dep_link_options) end

            end

            -- Include dirs
            if other_proj.inc_dirs then includedirs(other_proj.inc_dirs) end
            local platform_inc_dirs = find_table_with_platform(other_proj, "inc_dirs")
            if platform_inc_dirs then includedirs(platform_inc_dirs) end

            -- We also need link dirs
            if proj.kind ~= "StaticLib" then
              if other_proj.lib_dirs then linkdirs(other_proj.lib_dirs) end
              local platform_dep_lib_dirs = find_table_with_platform(other_proj, "lib_dirs")
              if platform_dep_lib_dirs then libdirs(platform_dep_lib_dirs) end
            end

            -- Preprocessor
            if other_proj.defines then defines(other_proj.defines) end
            local platform_defines = find_table_with_platform(other_proj, "defines")
            if platform_defines then defines(platform_defines) end

          end
        end

      end

    end

    buildoptions(proj.buildoptions)
    buildoptions('-x c++')

    -- Temp fix to programatically copy assets
    if proj.kind == "WindowedApp" and projects then

      for j, asset_proj in ipairs(projects) do
        if asset_proj.asset_dir then
          postbuildcommands("ditto ${SRCROOT}/".. asset_proj.asset_dir .." ${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/assets/");
        end
        -- postbuildcommands("ditto ${SRCROOT}/../assets/ ${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/assets/");
      end
    end

    -- Global build options
    if project_defaults.buildoptions then buildoptions(project_defaults.buildoptions) end

    local plaform_project_default_buildopts = find_table_with_platform(project_defaults, "buildoptions")
    if plaform_project_default_buildopts then buildoptions(plaform_project_default_buildopts) end

    local output = "../../output/"
    if solution_data.output then output = solution_data.output end

    configuration({"Debug"})
    defines({"DEBUG"})

    targetdir(output .. "debug/")
    objdir(output .. "objects/")

    if project_defaults.defines then defines(project_defaults.defines); end

    local platform_project_default_defines = find_table_with_platform(project_defaults, "defines")
    if platform_project_default_defines then defines(platform_project_default_defines) end

    flags({"Symbols", "Unicode"})
    flags(project_defaults.flags)

    local rtti = false
    if proj.lang_settings then
      if proj.lang_settings.rtti then rtti = true end
    end

    if rtti ~= true then
      flags("NoRTTI"); -- deprecated premake5
    end

    local exceptions = false
    if proj.lang_settings then
      if proj.lang_settings.exceptions then exceptions = true end
    end
    if exceptions ~= true then
      flags("NoExceptions") -- deprecated premake5
    end

    -- Release

    configuration({"Release"})
    defines({"NDEBUG", "NIMGUI"})

    targetdir(output .. "release/")
    objdir(output .. "objects/")

    if proj.targetdir then targetdir(proj.targetdir) end

    if project_defaults.defines then defines(project_defaults.defines); end

    local platform_project_default_defines = find_table_with_platform(project_defaults, "defines")
    if platform_project_default_defines then defines(platform_project_default_defines) end

    flags { "Optimize", "Unicode" }
    flags(project_defaults.flags)

    local rtti = false
    if proj.lang_settings then
      if proj.lang_settings.rtti then rtti = true end
    end

    if rtti ~= true then
      flags("NoRTTI"); -- deprecated premake5
    end

    local exceptions = false
    if proj.lang_settings then
      if proj.lang_settings.exceptions then exceptions = true end
    end
    if exceptions ~= true then
      flags("NoExceptions") -- deprecated premake5
    end

  end

end
