#include <platform/vs2017.h>
#include <internal/types.h>
#include <internal/common.h>
#include <internal/array.h>
#include <sedition/sed.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <experimental/filesystem>


struct msvs_config
{
	std::string name;
	std::string display_name;
	std::string arch_name;
	std::string toolset;

	GUID guid;
};


struct msvs_filter
{
	GUID guid;
	std::string path;
	std::string name;
};


struct msvs_file
{
	std::string filter_name;
	std::string path;
	std::string filename;
};


struct msvs_project
{
	struct GUID guid;
	std::string name;
	std::string path;
	std::string filename;

	std::vector<msvs_config> configs;
	std::vector<msvs_filter> filters;
	std::vector<msvs_file> files;
};


struct msvs_solution
{
	struct GUID guid;
	std::string name;
	std::string path;
	std::string filename;

	std::vector<msvs_config> configs;
	std::vector<msvs_project> projects;
};


namespace {

	auto get_index_from_id = [](const auto &container, int id)
	{
		int index = 0;

		for (const auto &f : container)
		{
			if (f.id == id)
			{
				return index;
			}

			++index;
		}

		return -1;
	};

}


void sed_generate_vs2017(
	std::vector<Solution> &sols,
	std::vector<Project> &projs,
	std::vector<Config> &confs
)
{
	/* generate windows data */
	std::vector<msvs_config> msvs_configs;
	msvs_configs.reserve(confs.size());

	for (const auto &conf : confs)
	{
		msvs_configs.emplace_back(msvs_config{});

		auto &config = msvs_configs.back();

		config.name = conf.name;
		config.display_name = "Win32";
		config.arch_name = "x86";
		config.toolset = "v141";
	}

	std::vector<msvs_project> msvs_projects;
	msvs_projects.reserve(projs.size());

	for (const auto &proj : projs)
	{
		msvs_projects.emplace_back(msvs_project{});

		auto &project = msvs_projects.back();

		project.name = proj.name;
		project.path = proj.path;
		project.filename = proj.name + ".vcxproj";
	}

	std::vector<msvs_solution> msvs_solutions;
	msvs_solutions.reserve(sols.size());

	for (const auto &sol : sols)
	{
		msvs_solutions.emplace_back(msvs_solution{});

		auto &solution = msvs_solutions.back();
		solution.name = sol.name;
		solution.filename = sol.name + ".sln";
		solution.path = sol.path;
		
		/* add solution configs */
		for(const auto &i : sol.configs)
		{
			for (int c = 0; c < confs.size(); ++c)
			{
				if (confs[c].id.value == i)
				{
					solution.configs.emplace_back(msvs_configs[c]);
				}
			}
		}

		for (auto p : sol.projects)
		{
			/* add this project */
			for (int i = 0; i < projs.size(); ++i)
			{
				if (projs[i].id.value == p)
				{
					const Project &proj = projs[i];

					solution.projects.emplace_back(msvs_projects[i]);

					msvs_project &msvs_proj = solution.projects.back();

					/* project configs */
					for(const auto &con : solution.configs)
					{
						solution.projects.back().configs.emplace_back(con);
					}

					/* files */
					{
						std::map<std::string, msvs_filter> filters;

						for (auto &f : proj.files)
						{
							if (filters.find(f.path) == std::end(filters))
							{
								filters[f.path] = msvs_filter{};
							}

							msvs_file new_file;
							new_file.filename = f.name;
							new_file.path = f.path;
							new_file.filter_name = f.path;

							msvs_proj.files.emplace_back(new_file);
						}
					}
				}
			}
		}
	}

	/* generate files */
	struct msvs_files
	{
		std::string filename;
		std::string path;
		std::string contents;
	};

	std::vector<msvs_files> files;

	/* create solution files */
	for (auto &sol : msvs_solutions)
	{
		std::string sol_contents;
		sol_contents.append("Microsoft Visual Studio Solution File, Format Version 12.00\n");
		sol_contents.append("# Visual Studio 15 - Generated with Sed\n");
		sol_contents.append("VisualStudioVersion = 15.0.27130.2027\n");
		sol_contents.append("MinimumVisualStudioVersion = 10.0.40219.1\n");

		for (auto &proj : sol.projects)
		{
			sol_contents.append("Project(\"{");
			sol_contents.append(proj.guid.str);
			sol_contents.append("}\") = \"");
			sol_contents.append(proj.name);
			sol_contents.append("\", ");

			sol_contents.append(proj.path);
			sol_contents.append("\\");
			
			sol_contents.append(proj.name);
			sol_contents.append(".vcxproj\", {");
			sol_contents.append(proj.guid.str);
			sol_contents.append("}\n");

			sol_contents.append("EndProject\n");
		}

		sol_contents.append("Global\n");

		/* configs */
		sol_contents.append("  GlobalSection(SolutionConfigurationPlatforms) = preSolution\n");

		for (const auto &conf : sol.configs)
		{
			sol_contents.append("    ");
			sol_contents.append(conf.name);
			sol_contents.append("|");
			sol_contents.append(conf.arch_name);
			sol_contents.append(" = ");
			sol_contents.append(conf.name);
			sol_contents.append("|");
			sol_contents.append(conf.arch_name);
			sol_contents.append("\n");
		}

		sol_contents.append("	EndGlobalSection\n");

		for (const auto &proj : sol.projects)
		{
			for (const auto &conf : proj.configs)
			{
				sol_contents.append("    {");
				sol_contents.append(proj.guid.str);
				sol_contents.append("}.");
				sol_contents.append(conf.name);
				sol_contents.append("|");
				sol_contents.append(".ActiveCfg = ");
				sol_contents.append(conf.name);
				sol_contents.append("|");
				sol_contents.append(conf.display_name);
				sol_contents.append("\n");

				sol_contents.append("    {");
				sol_contents.append(proj.guid.str);
				sol_contents.append("}.");
				sol_contents.append(conf.name);
				sol_contents.append("|");
				sol_contents.append(conf.arch_name);
				sol_contents.append(".Build.0 = ");
				sol_contents.append(conf.name);
				sol_contents.append("|");
				sol_contents.append(conf.display_name);
				sol_contents.append("\n");
			}
		}

		sol_contents.append("  EndGlobalSection\n");

		sol_contents.append("  GlobalSection(SolutionProperties) = preSolution\n");
		sol_contents.append("    HideSolutionNode = FALSE\n");
		sol_contents.append("  EndGlobalSection\n");

		sol_contents.append("  GlobalSection(ExtensibilityGlobals) = postSolution\n");
		sol_contents.append("    SolutionGuid = {");
		sol_contents.append(sol.guid.str);
		sol_contents.append("}\n");
		sol_contents.append("  EndGlobalSection\n");

		sol_contents.append("EndGlobal\n");

		/* create project files */
		for (auto &proj : sol.projects)
		{
			std::string proj_contents;

			proj_contents.append("<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n");
			proj_contents.append("<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n");

			proj_contents.append("  <ItemGroup Label=\"ProjectConfigurations\">\n");

			for (auto &conf : proj.configs)
			{
				proj_contents.append("		<ProjectConfiguration  Include=\"");
				proj_contents.append(conf.name);
				proj_contents.append("|");
				proj_contents.append(conf.display_name);
				proj_contents.append("\">\n");
				proj_contents.append("			<Configuration>");
				proj_contents.append(conf.name);
				proj_contents.append("</Configuration>\n");
				proj_contents.append("			<Platform>");
				proj_contents.append(conf.display_name);
				proj_contents.append("</Platform>\n");
				proj_contents.append("		<ProjectConfiguration>\n");
			}

			proj_contents.append("  </ItemGroup>\n");

			proj_contents.append("  <PropertyGroup Label=\"Globals\">\n");
			proj_contents.append("    <VCProjectVersion>15.0</VCProjectVersion>\n");
			proj_contents.append("    <ProjectGuid>{");
			proj_contents.append(sol.projects.begin()->guid.str);
			proj_contents.append("}</ProjectGuid>\n");
			proj_contents.append("    <RootNamespace>");
			proj_contents.append(sol.projects.begin()->name);
			proj_contents.append("</RootNamespace>\n");
			proj_contents.append("	  <WindowsTargetPlatformVersion>10.0.16299.0</WindowsTargetPlatformVersion>\n");
			proj_contents.append("  </PropertyGroup>\n");

			proj_contents.append("<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n");

			for (auto &conf : proj.configs)
			{
				proj_contents.append("  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)' == '");
				proj_contents.append(conf.name);
				proj_contents.append("|");
				proj_contents.append(conf.display_name);
				proj_contents.append("'\" Label=\"Configuration\">\n");
				proj_contents.append("    <ConfigurationType>Application</ConfigurationType>\n");
				proj_contents.append("    <UseDebugLibraries>true</UseDebugLibraries>\n");
				proj_contents.append("    <PlatformToolset>v141</PlatformToolset>\n");
				proj_contents.append("    <CharacterSet>MultiByte</CharacterSet>\n");
				proj_contents.append("  </PropertyGroup>\n");
			}

			proj_contents.append("  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n");

			proj_contents.append("  <ImportGroup Label=\"ExtensionSettings\">\n");
			proj_contents.append("  </ImportGroup>\n");

			proj_contents.append("  <ImportGroup Label=\"Shared\">\n");
			proj_contents.append("  </ImportGroup>\n");

			proj_contents.append("  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n");
			proj_contents.append("  <ImportGroup Label=\"ExtensionSettings\">\n");
			proj_contents.append("  </ImportGroup>\n");
			proj_contents.append("  <ImportGroup Label=\"Shared\">\n");
			proj_contents.append("  </ImportGroup>\n");

			for (auto &conf : proj.configs)
			{
				proj_contents.append("  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)' == '");
				proj_contents.append(conf.name);
				proj_contents.append("|");
				proj_contents.append(conf.display_name);
				proj_contents.append("'\">\n");

				proj_contents.append("    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n");
				proj_contents.append("  </ImportGroup>\n");
			}

			proj_contents.append("  <PropertyGroup Label=\"UserMacros\" />\n");
			proj_contents.append("  <PropertyGroup />\n");

			for (auto &conf : proj.configs)
			{
				proj_contents.append("  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)' == '");
				proj_contents.append(conf.name);
				proj_contents.append("|");
				proj_contents.append(conf.display_name);
				proj_contents.append("'\">\n");

				proj_contents.append("    <ClCompile>\n");
				proj_contents.append("      <WarningLevel>Level3</WarningLevel>\n");
				proj_contents.append("      <Optimization>Disabled</Optimization>\n");
				proj_contents.append("      <SDLCheck>true</SDLCheck>\n");
				proj_contents.append("      <ConformanceMode>true</ConformanceMode>\n");
				proj_contents.append("    </ClCompile>\n");
				proj_contents.append("  <ItemDfinitionGroup>\n");
			}

			proj_contents.append("  <ItemGroup>\n");
			proj_contents.append("  </ItemGroup>\n");
			proj_contents.append("  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n");
			proj_contents.append("  <ImportGroup Label=\"ExtensionTargets\">\n");
			proj_contents.append("  </ImportGroup>\n");

			proj_contents.append("</Project>\n");
			proj_contents.append("");


			files.emplace_back(msvs_files{ proj.filename, proj.path, proj_contents });
		}

		/* project filters */
		for (auto &proj : sol.projects)
		{
			std::string filter_contents;

			filter_contents.append("<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n");
			filter_contents.append("<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n");

			

			filter_contents.append("</Project>\n");
			
			std::string name = proj.name + ".vcxproj.filters";

			files.emplace_back(msvs_files{ name, proj.path, filter_contents });
		}

		files.emplace_back(msvs_files{ sol.filename, sol.path, sol_contents });
	}

	/* write out files */
	for (auto &file : files)
	{
		if (!std::experimental::filesystem::exists(file.path)) {
			std::experimental::filesystem::create_directory(file.path);
		}

		std::ofstream out_file;
		out_file.open(file.path + file.filename);

		if (out_file.is_open())
		{
			out_file << file.contents;
		}
		out_file.close();
	}
}
