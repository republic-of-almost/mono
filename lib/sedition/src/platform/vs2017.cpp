#include <platform/vs2017.h>
#include <internal/types.h>
#include <internal/common.h>
#include <internal/array.h>
#include <sedition/sed.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>


struct msvs_config
{
	std::string name;
	std::string display_name;
	std::string arch_name;
	std::string toolset;

	GUID guid;
};


struct msvs_project
{
	struct GUID guid;
	std::string name;
	std::string path;
	std::string filename;

	std::vector<msvs_config> configs;
};


struct msvs_solution
{
	struct GUID guid;
	std::string name;
	std::string path;
	std::string filename;

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
		
		for (const auto p : sol.projects)
		{
			/* add this project */
			for (int i = 0; i < projs.size(); ++i)
			{
				if (projs[i].id.value == p)
				{
					solution.projects.emplace_back(msvs_projects[i]);
				}
			}

			for (int i = 0; i < confs.size(); ++i)
			{
				
			}
		}
	}


	/* generate vs2017 */
	//int sol_count = sed_array_size(sols);

	//int i;

	//for (i = 0; i < sol_count; ++i)
	//{
	//	struct Solution *sol = &sols[i];

	//	/* generate solutions */
	//	{
	//		char *sol_filename = sed_string(sol->path);
	//		sol_filename = sed_string_append(sol_filename, sol->name);
	//		sol_filename = sed_string_append(sol_filename, ".sln");

	//		sed_mkdir(sol->path);

	//		sed_file_create(sol_filename);

	//		sed_file_append("Microsoft Visual Studio Solution File, Format Version 12.00\n");
	//		sed_file_append("# Visual Studio 15 - Generated with Sed\n");
	//		sed_file_append("VisualStudioVersion = 15.0.27130.2027\n");
	//		sed_file_append("MinimumVisualStudioVersion = 10.0.40219.1\n");

	//		/* list projects? */
	//		int p;
	//		int proj_count = sed_array_size(projs);

	//		for (p = 0; p < proj_count; ++p)
	//		{
	//			struct Project *proj = &projs[p];

	//			sed_file_append("Project(\"{");
	//			sed_file_append(proj->guid.str);
	//			sed_file_append("}\") = \"");
	//			sed_file_append(proj->name);
	//			sed_file_append("\", ");

	//			if (proj->path)
	//			{
	//				sed_file_append(proj->path);
	//				sed_file_append("\\");
	//			}

	//			sed_file_append(proj->name);
	//			sed_file_append(".vcxproj\", {");
	//			sed_file_append(proj->guid.str);
	//			sed_file_append("}\n");
	//		}

	//		sed_file_append("EndProject\n");

	//		sed_file_append("Global\n");

	//		/* configs */
	//		sed_file_append("  GlobalSection(SolutionConfigurationPlatforms) = preSolution\n");
	//		
	//		int c;
	//		int conf_count = sed_array_size(confs);

	//		for (c = 0; c < conf_count; ++c)
	//		{
	//			struct Config *conf = &confs[c];

	//			sed_file_append("    ");
	//			sed_file_append(conf->name);
	//			sed_file_append("|x86 = ");
	//			sed_file_append(conf->name);
	//			sed_file_append("|x86\n");
	//		}

	//		sed_file_append("  EndGlobalSection\n");
	//		
	//		sed_file_append("  GlobalSection(PorjectConfigurationPlatforms) = postSolution\n");
	//		
	//		for (p = 0; p < proj_count; ++p)
	//		{
	//			const struct Project *proj = &projs[p];

	//			for (c = 0; c < conf_count; ++c)
	//			{
	//				struct Config *conf = &confs[c];

	//				sed_file_append("    {");
	//				sed_file_append(proj->guid.str);
	//				sed_file_append("}.");
	//				sed_file_append(conf->name);
	//				sed_file_append("|x86.ActiveCfg = ");
	//				sed_file_append(conf->name);
	//				sed_file_append("|x86\n");

	//				sed_file_append("    {");
	//				sed_file_append(proj->guid.str);
	//				sed_file_append("}.");
	//				sed_file_append(conf->name);
	//				sed_file_append("|x86.Build.0 = ");
	//				sed_file_append(conf->name);
	//				sed_file_append("|x86\n");
	//			}
	//		}

	//		sed_file_append("  EndGlobalSection\n");

	//		sed_file_append("  GlobalSection(SolutionProperties) = preSolution\n");
	//		sed_file_append("    HideSolutionNode = FALSE\n");
	//		sed_file_append("  EndGlobalSection\n");

	//		sed_file_append("  GlobalSection(ExtensibilityGlobals) = postSolution\n");
	//		sed_file_append("    SolutionGuid = {");
	//		sed_file_append(sol->guid.str);
	//		sed_file_append("}\n");
	//		sed_file_append("  EndGlobalSection\n");

	//		sed_file_append("EndGlobal\n");

	//		sed_file_close();
	//	}

	//	/* projects */
	//	{
	//		int proj_count = sed_array_size(projs);
	//		int p;

	//		int conf_count = sed_array_size(confs);
	//		int c;

	//		for (p = 0; p < proj_count; ++p)
	//		{
	//			struct Project *proj = &projs[p];

	//			char *proj_filename = sed_string(proj->path);
	//			proj_filename = sed_string_append(proj_filename, proj->name);
	//			proj_filename = sed_string_append(proj_filename, ".vcxproj");

	//			sed_mkdir(proj->path);

	//			sed_file_create(proj_filename);

	//			sed_file_append("<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n");
	//			sed_file_append("<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n");

	//			/* project configs */
	//			sed_file_append("  <ItemGroup Label=\"ProjectConfigurations\">\n");
	//			sed_file_append("  </ItemGroup>\n");

	//			/* globals */
	//			sed_file_append("  <PropertyGroup Label=\"Globals\">\n");
	//			sed_file_append("    <VCProjectVersion>15.0</VCProjectVersion>\n");
	//			sed_file_append("    <ProjectGuid>{");
	//			sed_file_append(proj->guid.str);
	//			sed_file_append("}</ProjectGuid>\n");
	//			sed_file_append("    <RootNamespace>");
	//			sed_file_append(sol->name);
	//			sed_file_append("</RootNamespace>\n");
	//			sed_file_append("    <WindowsTargetPlatformVersion>10.0.16299.0</WindowsTargetPlatformVersion>\n");
	//			sed_file_append("  </PropertyGroup>\n");

	//			/* import */
	//			sed_file_append("  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n");

	//			/* configs */
	//			sed_file_append("  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)' == 'Debug|Win32'\" Label=\"Configuration\">\n");
	//			
	//			for (c = 0; c < conf_count; ++c)
	//			{
	//				sed_file_append("    <ConfigurationType>Application</ConfigurationType>\n");
	//				sed_file_append("    <UseDebugLibraries>true</UseDebugLibraries>\n");
	//				sed_file_append("    <PlatformToolset>v141</PlatformToolset>\n");
	//				sed_file_append("    <CharacterSet>MultiByte</CharacterSet>\n");
	//			}

	//			sed_file_append("  </PropertyGroup>\n");

	//			/* globals */
	//			sed_file_append("  <ItemGroup Label=\"Globals\">\n");
	//			sed_file_append("  </ItemGroup>\n");

	//			/* import */
	//			sed_file_append("  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n");

	//			/* extension settings */
	//			sed_file_append("  <ImportGroup Label=\"ExtensionSettings\">\n");
	//			sed_file_append("  </ImportGroup>\n");

	//			/* shared */
	//			sed_file_append("  <ImportGroup Label=\"Shared\">\n");
	//			sed_file_append("  </ImportGroup>\n");

	//			/* config prop sheets */

	//			for (c = 0; c < conf_count; ++c)
	//			{
	//				sed_file_append("  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)' == 'Debug|Win32'\">\n");
	//				sed_file_append("    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n");
	//				sed_file_append("  </ImportGroup>\n");
	//			}
	//			/* user macros */
	//			sed_file_append("  <PropertyGroup Label=\"UserMacros\" />\n");
	//			sed_file_append("  <PropertyGroup / >\n");

	//			/* config settings */

	//			for (c = 0; c < conf_count; ++c)
	//			{
	//				sed_file_append("  <ItemDefinitionGroup Condition = \"'$(Configuration)|$(Platform)' == 'Debug|Win32'\">\n");
	//				sed_file_append("    <ClCompile>\n");
	//				sed_file_append("      <WarningLevel>Level3</WarningLevel>\n");
	//				sed_file_append("      <Optimization>Disabled</Optimization>\n");
	//				sed_file_append("      <SDLCheck>true</SDLCheck>\n");
	//				sed_file_append("      <ConformanceMode>true</ConformanceMode>\n");
	//				sed_file_append("    </ClCompile>\n");
	//				sed_file_append("  </ItemDefinitionGroup>\n");
	//			}

	//			/* compile */
	//			sed_file_append("  <ItemGroup>\n");
	//			sed_file_append("    <ClCompile Include=\"main.cpp\" />\n");
	//			sed_file_append("  </ItemGroup>\n");

	//			/* includes */
	//			sed_file_append("  <ItemGroup>\n");
	//			sed_file_append("    <ClInclude Include=\"main.cpp\" />\n");
	//			sed_file_append("  </ItemGroup>\n");

	//			/* footer */
	//			sed_file_append("  <Import Project = \"$(VCTargetsPath)\\Microsoft.Cpp.targets\"/>\n");
	//			sed_file_append("  <ImportGroup Label = \"ExtensionTargets\">\n");
	//			sed_file_append("  </ImportGroup>\n");
	//			sed_file_append("</Project>\n");

	//			sed_file_close();
	//		}
	//	}
	//}
}
