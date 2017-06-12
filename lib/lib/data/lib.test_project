#!lua

--[[
  Utilities Test Project Description
]]--

table.insert(
  projects,
  {
    name     = "UtilitiesTest",
    kind     = "ConsoleApp",
    language = "C++",
    location = make.get_proj_root(),
    no_link  = true,

    src_files = {
      make.add_headers(make.get_proj_root()),
      make.add_src(make.get_proj_root()),
      make.add_doc(make.get_proj_root()),
    },

    inc_dirs = {
      make.get_proj_root() .. "./",
    },

    project_dependencies = {
      "Utilities",
    },
  }
)
