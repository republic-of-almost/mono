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
  if os.get() == "macosx" then
    return {
      dir .. "**.cpp",
      dir .. "**.cc",
      dir .. "**.mm",
      dir .. "**.c",
      dir .. "**.m",
    }
  else
    return {
      dir .. "**.cpp",
      dir .. "**.cc",
      dir .. "**.c",
    }
  end
end


-- Adds source file patterns
function
make.add_src(dir)
  if os.get() == "macosx" then
    return {
      dir .. "**.cpp",
      dir .. "**.cc",
      dir .. "**.mm",
      dir .. "**.c",
      dir .. "**.m",
    }
  else
    return {
      dir .. "**.cpp",
      dir .. "**.cc",
      dir .. "**.c",
    }
  end
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


-- Adds inline file patterns
function
make.add_inlines(dir)
  return {
    dir .. "**.inl",
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


----------------------------------------------------------------- [ CONFIGS ] --


code_configs = {

  -- Development --
  -- Focus's on developer ease
  -- no errors on warnings etc.
  {
    name = "development",
    flags = {"Symbols"},
    defines = {"DEBUG", "NIL_DEVELOPMENT", "NIL_PEDANTIC"},
  },

  -- Stage --
  -- Release build with symbols.
  -- Errors on warnings are now on.
  -- Debug flags still enabled.
  {
    name = "staging",
    flags = {"Optimize", "Symbols"},
    defines = {"DEBUG", "NIL_STAGE", "NIL_PEDANTIC"},
  },

  -- Release --
  -- Final build.
  {
    name = "release",
    flags = {"Optimize"},
    defines = {"NDEBUG", "NIL_RELEASE", "NIMGUI", "NDEBUGLINES"},
  },
}


------------------------------------------------------ [ SOLUTION GENERATOR ] --


-- Generates a solution.
function
make.create_solution(solution_data, project_defaults, projects)

  -- Create the solution for the project.
  print("Creating Solution")

  solution(solution_data.name)
  location("./")

  if(os.get() == "windows") then
    characterset("MBCS")
  end

  -- Create Configs --
  local configs = {}

  for i, config in ipairs(code_configs) do
    print(config.name)
    table.insert(configs, config.name)
  end

  configurations(configs)

  -- Create the projects
  -- for i, proj in ipairs(arg) do -- old way.

  for i, proj in ipairs(projects) do

    print("Building project: " .. proj.name)

    -- Fill in the default information.
    if not proj.kind                then proj.kind = "ConsoleApp"        end
    if not proj.lib_directories     then proj.lib_directories = {""}     end
    if not proj.include_directories then proj.include_directories = {""} end

    -- Generate the project data.
    project(proj.name)
    location(proj.location)
    kind(proj.kind)

    if(proj.language == "C++") then
      language("C++")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c++14");
      end
    elseif(proj.language == "C++11") then
      language("C++")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c++11");
      end
    elseif(proj.language == "C++14") then
      language("C++")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c++14");
      end
    elseif(proj.language == "C++17") then
      language("C++")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c++17");
      end
    elseif(proj.language == "C") then
      language("C")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c99");
      end
    elseif(proj.language == "C89") then
      language("C")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c89");
      end
    elseif(proj.language == "C99") then
      language("C")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c99");
      end
    elseif(proj.language == "C11") then
      language("C")
      if os.get() == "macosx" or os.get() == "linux" then
        buildoptions("-std=c11");
      end
    end

    -- Thie function takes a string that represents a field
    -- to search in the table. it will then append the premakes
    -- platform name to the end, and search for that field.
    function
    find_table_with_platform(proj, string)
      local result_table = {}
      local call_str = "result_table = proj." .. string .. "_" .. os.get()

      -- Need to watch this - loadstring is depreated, but premake's version of lua is old.
      --local chunk = loadstring(call_str)
      --setfenv( chunk, { result_table = result_table, proj = proj } )
      --chunk()

      -- This is new method, should premake's lua version change.
      local my_env = { result_table = result_table, proj = proj }
      load(call_str, nil, nil, my_env)()

      --return getfenv(chunk).result_table
      return result_table
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
    if proj.src_files_exclude then excludes(proj.src_files_exclude) end

    local platform_exclude = find_table_with_platform(proj, "src_files_exclude")
    if platform_exclude then excludes(platform_exclude) end

    -- Public include dirs
    if proj.public_inc_dirs then includedirs(proj.public_inc_dirs) end

    local platform_inc_dirs = find_table_with_platform(proj, "public_inc_dirs")
    if platform_inc_dirs then includedirs(platform_inc_dirs) end

    -- Private include dirs
    if proj.private_inc_dirs then includedirs(proj.private_inc_dirs) end

    local platform_inc_dirs = find_table_with_platform(proj, "private_inc_dirs")
    if platform_inc_dirs then includedirs(platform_inc_dirs) end

    -- Library directories
    if proj.lib_dirs then libdirs(proj.lib_dirs) end

    local platform_lib_dirs = find_table_with_platform(proj, "lib_dirs")
    if platform_lib_dirs then libdirs(platform_lib_dirs) end

    -- Link options
    if proj.linkoptions then  linkoptions(proj.linkoptions) end

    local platform_link_options = find_table_with_platform(proj, "linkoptions")
    if platform_link_options then linkoptions(platform_link_options) end

    -- Warnings
    if proj.disable_warning then disablewarnings(proj.disable_warning) end

    local platform_disable_warning = find_table_with_platform(proj, "disable_warning")
    if platform_disable_warning then disablewarnings(platform_disable_warning) end

    -- Links and Link dependencies
    if proj.links then links(proj.links) end

    -- dependencies --
    -- Helper to allow us to recursivly get the dependencies --
    function
    get_dependencies(proj, orig_proj, padding)

      -- If we have project dependencies then we need
      -- to check if that project specifies any links
      -- as dependencies.
      if proj.project_dependencies then

        -- Loop through each of the dependencies
        for i, dep in ipairs(proj.project_dependencies) do

          print(padding .. "  Dependency " .. dep)

          -- Loop through the projects we have been given.
          for j, other_proj in ipairs(projects) do

            -- If a match then check for links
            if dep == other_proj.name then

              -- Projects can be marked no link
              -- But still want to bring in header files etc.

              link = true;
              if other_proj.no_link == true then link = false end -- deprecated
              if other_proj.lang_settings then
                if other_proj.lang_settings.no_link == true then link = false end
              end

              if link and (orig_proj.kind ~= "StaticLib") then

                -- Add this project as a dep
                links(other_proj.name)

                -- Add listed project deps
                if other_proj.link_dependencies then
                  links(other_proj.link_dependencies)
                end

                -- Find platform deps
                local platform_dep_links = find_table_with_platform(other_proj, "link_dependencies")
                if platform_dep_links then links(platform_dep_links) end

                -- Add link option dependencies
                if other_proj.linkoption_dependencies then
                  linkoptions(other_proj.linkoption_dependencies)
                end

                -- Platform
                local platform_dep_link_options = find_table_with_platform(other_proj, "linkoption_dependencies")
                if platform_dep_link_options then
                  linkoptions(platform_dep_link_options)
                end

              end

              -- Include dirs
              if other_proj.public_inc_dirs then includedirs(other_proj.public_inc_dirs) end
              local platform_inc_dirs = find_table_with_platform(other_proj, "public_inc_dirs")
              if platform_inc_dirs then includedirs(platform_inc_dirs) end

              -- We also need link dirs
              if proj.kind ~= "StaticLib" then
                if other_proj.lib_dirs then libdirs(other_proj.lib_dirs) end
                local platform_dep_lib_dirs = find_table_with_platform(other_proj, "lib_dirs")
                if platform_dep_lib_dirs then libdirs(platform_dep_lib_dirs) end
              end

              -- Preprocessor
              if other_proj.defines then defines(other_proj.defines) end
              local platform_defines = find_table_with_platform(other_proj, "defines")
              if platform_defines then defines(platform_defines) end

              -- dependencies
              get_dependencies(other_proj, orig_proj, padding .. "  ")
            end
          end

        end

      end
    end

    get_dependencies(proj, proj, "  ")

    buildoptions(proj.buildoptions)

    -- Global build options --
    if proj.ignore_defaults ~= true then
      if project_defaults.buildoptions then buildoptions(project_defaults.buildoptions) end

      local plaform_project_default_buildopts = find_table_with_platform(project_defaults, "buildoptions")
      if plaform_project_default_buildopts then buildoptions(plaform_project_default_buildopts) end
    end

    local output = "../../output/"
    if solution_data.output then output = solution_data.output end

    -- Setup Configs --
    for j, config in ipairs(code_configs) do

      configuration(config.name)

      defines(config.defines)
      flags(config.flags)
      targetdir(output .. config.name .. "/")
      objdir(output .. "objects/")

      if proj.ignore_defaults ~= true then
        if project_defaults.defines then defines(project_defaults.defines) end

        local platform_project_default_defines = find_table_with_platform(project_defaults, "defines")
        if platform_project_default_defines then defines(platform_project_default_defines) end

        flags(project_defaults.flags)
      end

      -- RTTI --
      local rtti = false
      if proj.lang_settings then
        if proj.lang_settings.rtti then rtti = true end
      end

      if rtti ~= true then
        flags("NoRTTI"); -- deprecated premake5
      end

      -- Exceptions --
      local exceptions = false
      if proj.lang_settings then
        if proj.lang_settings.exceptions then exceptions = true end
      end

      if exceptions ~= true then
        flags("NoExceptions") -- deprecated premake5
      end

      -- Asset directories get copied to build --
      function
      copy_files(src_dir, dest_dir)

        -- MacOSX Copy --
        if os.get() == "macosx" then
          if proj.kind == "WindowedApp" then
            postbuildcommands("ditto " .. src_dir .." ${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/assets/");
          elseif proj.kind == "ConsoleApp" then
            postbuildcommands("ditto " .. src_dir .." ${CONFIGURATION_BUILD_DIR}/assets/");
          end
        -- Linux Copy --
        elseif os.get() == "linux" then
          if proj.kind == "WindowedApp" or proj.kind == "ConsoleApp" then
            postbuildcommands("cp -rf " .. src_dir .. "* " .. dest_dir .. config.name .. "/assets/" .. " 2>/dev/null || :");
          end
        -- Windows Copy --
        elseif os.get() == "windows" then
          if proj.kind == "WindowedApp" or proj.kind == "ConsoleApp" then

            win_src_dir = src_dir
            win_dest_dir = dest_dir

            if win_src_dir[-1] ~= "/" then win_src_dir = win_src_dir .. "/" end
            if win_dest_dir[-1] ~= "/" then win_dest_dir = win_dest_dir .. "/" end

            win_src_dir = string.gsub(src_dir, "/", "\\")
            win_dest_dir = string.gsub(dest_dir, "/", "\\") .. config.name .. "\\assets\\"

            postbuildcommands("xcopy /s/z/y \"" .. win_src_dir .. "*.*\" \"" .. win_dest_dir .. "*.*\"");
          end
        end

      end

      for j, asset_proj in ipairs(projects) do
        if(asset_proj.assets) then
          for k, asset_dir in ipairs(asset_proj.assets) do
            if asset_dir then
              copy_files(asset_dir, make.get_proj_root() .. "../../output/")
            end
          end
        end
      end

    end

  end

end


function
make.find_missing_dependencies(all_deps, curr_projs)

  deps = {}

  for i, proj in ipairs(curr_projs) do
    if proj.project_dependencies then

      for j, dep in ipairs(proj.project_dependencies) do

        exists = false

        -- Check current dependencies.
        for k, other_proj in ipairs(curr_projs) do
          if dep.name == other_proj.name then
            exists = true
            break
          end
        end

        -- Look at other deps
        if exists == false then
          for k, other_proj in ipairs(all_deps) do
            if dep.name == other_proj.name then
              print("Missing dep found.")
              table.insert(deps, other_proj)
              exists = true;
              break
            end
          end

          if(exists == false) then
            print("Failed to resolve dep")
          end
        end

      end
    end
  end

end
