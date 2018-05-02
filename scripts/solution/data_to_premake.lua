#!lua

make = {} -- namespace

os_target = "unknown"

if os.target ~= nil then
  os_target = os.target()
else
  os_target = os.get()
end


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
  if os_target == "macosx" then
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
  if os_target == "macosx" then
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
    symbols = true,
    simd = false,
    fast_float = true,
    extra_warnings = true,
  },

  -- Stage --
  -- Release build with symbols.
  -- Errors on warnings are now on.
  -- Debug flags still enabled.
  {
    name = "staging",
    symbols = true,
    simd = true,
    optimize = true,
    fast_float = true,
    extra_warnings = true,
  },

  -- Release --
  -- Final build.
  {
    name = "release",
    symbols = false,
    simd = true,
    optimize = true,
    fast_float = true,
    extra_warnings = true,

    defines = {"NDEBUG"},
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

  if(os_target == "windows") then
    characterset("MBCS")
  end

  -- Create Configs --
  local configs = {}

  for i, config in ipairs(code_configs) do
    --print(config.name)
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
    location(proj.base_location .. proj.location)
    kind(proj.kind)

    if(proj.language == "C++") then
      language("C++")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c++14");
      end
    elseif(proj.language == "C++11") then
      language("C++")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c++11");
      end
    elseif(proj.language == "C++14") then
      language("C++")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c++14");
      end
    elseif(proj.language == "C++17") then
      language("C++")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c++17");
      end
    elseif(proj.language == "C") then
      language("C")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c99");
      end
    elseif(proj.language == "C89") then
      language("C")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c89");
      end
    elseif(proj.language == "C99") then
      language("C")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c99");
      end
    elseif(proj.language == "C11") then
      language("C")
      if os_target == "macosx" or os_target == "linux" then
        buildoptions("-std=c11");
      end
    end

    -- Thie function takes a string that represents a field
    -- to search in the table. it will then append the premakes
    -- platform name to the end, and search for that field.
    -- This function can vanish
    function
    find_table_with_platform(proj, string)
      local result_table = {}
      local call_str = "result_table = proj." .. string .. "_" .. os_target

      -- Need to watch this - loadstring is depreated, but premake's version of lua is old.
      --local chunk = loadstring(call_str)
      --setfenv( chunk, { result_table = result_table, proj = proj } )
      --chunk()

      -- This is new method, should premake's lua version change.
      local my_env = { result_table = result_table, proj = proj }
      load(call_str, nil, "t", my_env)()

      result_table = proj[string .. "_" .. os_target]

      --dump(result_table)

      --return getfenv(chunk).result_table
      return result_table
    end

    function
    find_table(proj, ident)

      local results = {};

      local data = proj[ident];

      if data then
        results = table.join(results, data)
      end

      local platform_data = proj[ident .. "_" .. os_target]

      if platform_data then
        results = table.join(results, platform_data)
      end

      return results;

    end

    -- Src files
    local _files = find_table(proj, "src_files");

    for i,v in ipairs(find_table(proj, "src_files")) do
      _files[i] = proj.base_location .. "/" .. v
      --print("FILE: " .. proj.base_location .. "/" .. v)
    end

    files(_files)

    -- Excludes
    excludes(find_table(proj, "src_files_exclude"))

    -- include dirs (pub and private)
    local _inc_dirs = {};
    table.insert(_inc_dirs, proj.public_inc_dirs)
    table.insert(_inc_dirs, find_table_with_platform(proj, "public_inc_dirs"))
    table.insert(_inc_dirs, proj.private_inc_dirs)
    table.insert(_inc_dirs, find_table_with_platform(proj, "private_inc_dirs"))

    _inc_dirs = table.flatten(_inc_dirs)

    if _inc_dirs then
      for i,v in ipairs(_inc_dirs) do
        _inc_dirs[i] = proj.base_location .. "/" .. v
      end

      includedirs(_inc_dirs)
    end

    defines(find_table(proj, "defines"))
    libdirs(find_table(proj, "libdirs")) -- this might have a realitive path issue.
    linkoptions(find_table(proj, "linkoptions"))
    disablewarnings(find_table(proj, "disable_warning"))
    links(find_table(proj, "links"))

    -- dependencies --
    -- Helper to allow us to recursivly get the dependencies --
    asset_dependencies = {}
    table.insert(asset_dependencies, proj);

    function
    get_dependencies(proj, orig_proj, padding)

      -- If we have project dependencies then we need
      -- to check if that project specifies any links
      -- as dependencies.
      if proj.project_dependencies then

        -- Loop through each of the dependencies
        for i, dep in ipairs(proj.project_dependencies) do

          --print(padding .. "  Dependency " .. dep)

          -- Loop through the projects we have been given.
          for j, other_proj in ipairs(projects) do

            -- If a match then check for links
            if dep == other_proj.name then

              -- Projects can be marked no link
              -- But still want to bring in header files etc.

              table.insert(asset_dependencies, dep);

              link = true;
              if other_proj.no_link == true then link = false end -- deprecated
              if other_proj.lang_settings then
                if other_proj.lang_settings.no_link == true then link = false end
              end

              if link and (orig_proj.kind ~= "StaticLib") then

                links(other_proj.name)
                links(find_table(other_proj, "link_dependencies"))
                linkoptions(find_table(other_proj, "linkoption_dependencies"));

              end

              -- Include dirs
              local _dep_inc_dirs = {}
              table.insert(_dep_inc_dirs, other_proj.public_inc_dirs)
              table.insert(_dep_inc_dirs, find_table_with_platform(other_proj, "public_inc_dirs"))
              _dep_inc_dirs = table.flatten(_dep_inc_dirs)

              if _dep_inc_dirs then
                for i,v in ipairs(_dep_inc_dirs) do
                  _dep_inc_dirs[i] = other_proj.base_location .. "/" .. v
                end

                includedirs(_dep_inc_dirs)
              end


              if other_proj.public_inc_dirs then includedirs(other_proj.public_inc_dirs) end
              local platform_inc_dirs = find_table_with_platform(other_proj, "public_inc_dirs")
              if platform_inc_dirs then includedirs(platform_inc_dirs) end

              -- We also need link dirs
              if proj.kind ~= "StaticLib" then
                libdirs(find_table(other_proj, "lib_dirs"))
              end

              -- Preprocessor
              defines(find_table(other_proj, "defines"))

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

      if config.optimize then optimize("On") else optimize("Off") end
      if config.symbols then symbols("On") else symbols("Off") end
      if config.simd then vectorextensions("SSE2") end
      if config.fast_float then floatingpoint("Fast") end
      if config.extra_warnings then warnings("Extra") end

      targetdir(output .. config.name .. "/")
      objdir(output .. "objects/")

      if proj.ignore_defaults ~= true then
        if project_defaults.defines then defines(project_defaults.defines) end

        local platform_project_default_defines = find_table_with_platform(project_defaults, "defines")
        if platform_project_default_defines then defines(platform_project_default_defines) end
      end

      -- RTTI --
      local _enable_rtti = "Off"
      if proj.lang_settings then
        if proj.lang_settings.rtti then _enable_rtti = "On" end
      end

      rtti(_enable_rtti)

      -- Exceptions --
      local _enable_exceptions = "Off"
      if proj.lang_settings then
        if proj.lang_settings.exceptions then _enable_exceptions = "On" end
      end

      exceptionhandling(_enable_exceptions)

      -- Asset directories get copied to build --
      function
      copy_files(src_dir, dest_dir)

        -- MacOSX Copy --
        if os_target == "macosx" then
          if proj.kind == "WindowedApp" then
            postbuildcommands("ditto " .. src_dir .." ${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/assets/");
          elseif proj.kind == "ConsoleApp" then
            postbuildcommands("ditto " .. src_dir .." ${CONFIGURATION_BUILD_DIR}/assets/");
          end
        -- Linux Copy --
        elseif os_target == "linux" then
          if proj.kind == "WindowedApp" or proj.kind == "ConsoleApp" then
            postbuildcommands("cp -rf " .. src_dir .. "* " .. dest_dir .. config.name .. "/assets/" .. " 2>/dev/null || :");
          end
        -- Windows Copy --
        elseif os_target == "windows" then
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

      for j, asset_proj in ipairs(asset_dependencies) do
        if(asset_proj.assets) then
          for k, asset_dir in ipairs(asset_proj.assets) do
            if asset_dir then
              print(asset_dir)
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
