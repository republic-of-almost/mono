#include <sedition/sed.h>
#include <internal/array.h>
#include <internal/common.h>
#include <internal/types.h>
#include <stdio.h> /* testing */
#include <vector>
#include <assert.h>
#include <algorithm>


/* -------------------------------------------------------------- [ Data ] -- */


namespace {

	std::vector<Solution> solutions;
	std::vector<Project> projects;
	std::vector<Config> configs;

	auto find_item = [](auto &container, int id_to_find)
	{
		return std::find_if(
			std::begin(container),
			std::end(container),
			[id_to_find](auto i) { return i.id.value == id_to_find; });
	};
}

/* ---------------------------------------------------------- [ Solution ] -- */


int
sed_solution_create(const char *name)
{
	solutions.emplace_back(Solution{});
	solutions.back().name = name;

	return solutions.back().id.value;
}


void
sed_solution_add_project(int sol_id, int proj_id)
{
	/* find project */
	auto &proj_to_add = find_item(projects, proj_id);
	
	if (proj_to_add == std::end(projects))
	{
		return;
	}

	/* find solution */
	auto &sol_to_update = find_item(solutions, sol_id);
	
	if (sol_to_update == std::end(solutions))
	{
		return;
	}

	/* add solution if not added */
	auto &proj_ids = sol_to_update->projects;
	auto find_proj = std::find_if(std::begin(proj_ids), std::end(proj_ids), [proj_id](auto id) { return id == proj_id; });

	if (find_proj == std::end(proj_ids))
	{
		proj_ids.emplace_back(proj_id);
	}
}


void
sed_solution_add_config(int sol_id, int config_id)
{
	/* find project */
	auto &conf_to_add = find_item(configs, config_id);

	if (conf_to_add == std::end(configs))
	{
		return;
	}

	/* find solution */
	auto &sol_to_update = find_item(solutions, sol_id);

	if (sol_to_update == std::end(solutions))
	{
		return;
	}

	/* add solution if not added */
	auto &config_ids = sol_to_update->configs;
	auto find_proj = std::find_if(std::begin(config_ids), std::end(config_ids), [config_id](auto id) {return id == config_id; });

	if (find_proj == std::end(config_ids))
	{
		config_ids.emplace_back(config_id);
	}
}


void
sed_solution_set_path(int sol_id, const char *path)
{

}


/* ------------------------------------------------------------ [ Config ] -- */


int
sed_config_create(const char *name)
{
	configs.emplace_back(Config{});

	return configs.back().id.value;
}


void
sed_config_set_has_debug_symbols(int config_id, int symbols)
{
}


void
sed_config_set_optimisation_level(int config_id, int optim)
{
}


/* ----------------------------------------------------------- [ Project ] -- */


int
sed_project_create(const char *name)
{
	projects.emplace_back(Project{});
	projects.back().name = name;

	return projects.back().id.value;
}


void
sed_project_set_kind(int proj_id, int kind)
{
	
}


void
sed_project_set_language(int proj_id, int lang)
{
}


void
sed_project_add_file(int proj_id, const char *file)
{
}


void
sed_project_add_include_dir(int proj_id, const char *dir)
{
}


void
sed_project_add_library_dir(int proj_id, const char *dir)
{
}


void
sed_project_set_path(int proj_id, const char *path)
{
}


/* ----------------------------------------------------------- [ Execute ] -- */


#include <platform/xcode.h>
#include <platform/vs2017.h>


void
sed_execute(int platform)
{
  /* generate platform solution */
  /* sed_generate_xcode(solutions, projects, configs); */
  sed_generate_vs2017(solutions, projects, configs);
}


/* -------------------------------------------------------------- [ Main ] -- */


#ifndef SED_AS_LIB
int
main()
{
  /* testing */

  int solution = sed_solution_create("FooSol");
  sed_solution_set_path(solution, "../../../TestSolution/MyTest/");

  int config = sed_config_create("DebugConfig");

  int proj = sed_project_create("BarProj");
  sed_project_set_path(proj, "../../../TestSolution/MyTest/BarProj/");
  sed_project_set_language(proj, SED_LANG_C89);
  sed_project_set_kind(proj, SED_KIND_CONSOLE_APP);
  sed_project_add_file(proj, "../../src/foo.c");
  sed_project_add_file(proj, "../../include/foo.h");
  sed_project_add_include_dir(proj, "./include");
  sed_project_add_library_dir(proj, "/usr/local");

  sed_solution_add_config(solution, config);
  sed_solution_add_project(solution, proj);

  sed_execute(SED_PLAT_OUTPUT_ONLY);

  return 0;
}
#endif
