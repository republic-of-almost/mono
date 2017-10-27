#include <platform/xcode.h>
#include <internal/types.h>
#include <internal/common.h>
#include <internal/array.h>
#include <string.h>
#include <stdio.h>


void sed_generate_xcode(
  struct Solution *sols,
  struct Project *projs,
  struct Config *confs)
{
  /* generating xcode */
  int sol_count = sed_array_size(sols);

  int i;
  for(i = 0; i < sol_count; ++i)
  {
    struct Solution *sol = &sols[i];

    /* generate workspace */
    {
      /* project */
      {
        int proj_count = sed_array_size(sol->projects);
        int j;

        for(j = 0; j < proj_count; ++j)
        {
          struct Project *proj = &projs[sol->projects[i -1]];

          char *dir_name = sed_string(proj->name);
          
          dir_name = sed_string_append(dir_name, ".xcodeproj");

          sed_mkdir(dir_name);

          char *proj_file = sed_string(dir_name);
          proj_file = sed_string_append(proj_file, "/project.pbxproj");

          sed_file_create(proj_file);

          /* header */

          sed_file_append("// !$*UTF8*$!\n");
          sed_file_append("{\n");
          sed_file_append("\tarchiveVersion = 1;\n");
          sed_file_append("\tclasses = {\n");
          sed_file_append("\t};\n");
          sed_file_append("\tobjectVersion = 45;\n");
          sed_file_append("\tobjects = {\n\n");

          /* files */

          sed_file_append("/* Begin PBXBuildFile section */\n");
          sed_file_append("/* End PBXBuildFile section */\n\n");

          /* file refs */

          sed_file_append("/* Begin PBXFileReference section */\n");
          sed_file_append("/* End PBXFileReference section */\n\n");

          /* framework buildphase */

          sed_file_append("/* Begin PBXFrameworksBuildPhase section */\n");
          sed_file_append("/* End PBXFrameworksBuildPhase section */\n\n");

          /* group */

          sed_file_append("/* Begin PBXGroup section */\n");
          sed_file_append("/* End PBXGroup section */\n\n");

          /* native target */

          sed_file_append("/* Begin PBXNativeTarget section */\n");
          sed_file_append("/* End PBXNativeTarget section */\n\n");

          /* project */

          sed_file_append("/* Begin PBXProject section */\n");
          sed_file_append("/* End PBXProject section */\n\n");

          /* resources */

          sed_file_append("/* Begin PBXResourcesBuildPhase section */\n");
          sed_file_append("/* End PBXResourcesBuildPhase section */\n\n");

          /* sources */

          sed_file_append("/* Begin PBXSourcesBuildPhase section */\n");
          sed_file_append("/* End PBXSourcesBuildPhase section */\n\n");

          /* varient */

          sed_file_append("/* Begin PBXVariantGroup section */\n");
          sed_file_append("/* END PBXVariantGroup section */\n\n"); 

          /* config */

          sed_file_append("/* Begin XCBuildConfiguration section */\n");
          sed_file_append("/* End XCBuildConfiguration section */\n\n");

          /* config list */

          sed_file_append("/* Begin XCConfigurationList section */\n");
          sed_file_append("/* End XCConfigurationList section */\n\n");

          /* root */

          sed_file_append("rootObject = ");
          sed_file_append(" <insert_id> ");
          sed_file_append(" /* Project object */;\n\n");

          /* footer */

          sed_file_append("}\n");

          sed_file_close();

          sed_string_free(proj_file);
          sed_string_free(dir_name);
        }
      }

      /* workspace */
      {
        char *dir_name = sed_string(sol->name);
        dir_name = sed_string_append(dir_name, ".xcworkspace");

        sed_mkdir(dir_name);

        char *work_file = sed_string(dir_name);
        work_file = sed_string_append(work_file, "/contents.xcworkspacedata");

        sed_file_create(work_file);
        sed_file_append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        sed_file_append("<Workspace\n");
        sed_file_append("\tversion = \"1.0\">\n");
                
        int proj_count = sed_array_size(sol->projects);
        int j;

        for(j = 0; j < proj_count; ++j)
        {
          sed_file_append("\t<FileRef\n");
          sed_file_append("\t\tlocation = \"group=\">\n");
          sed_file_append("\t</FileRef>\n");
        }

        sed_file_append("</Workspace>\n");
        
        sed_file_close();

        sed_string_free(work_file);
        sed_string_free(dir_name);
      }
    } /* generate workspace */
  } /* for solutions */
} /* sed_generate_xcode */
