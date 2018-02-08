#include <platform/xcode.h>
#include <internal/types.h>
#include <internal/common.h>
#include <internal/array.h>
#include <sedition/sed.h>
#include <string.h>
#include <stdio.h>


/* generates ids for xcode */
void
sed_xcode_id(char *id) // ptr to 25 chars
{
  const char rand_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int char_count = sizeof(rand_chars) / sizeof(char);
  char_count--;

  int i;

  for(i = 0; i < 24; ++i)
  {
    id[i] = rand_chars[rand() % char_count];
  }
}


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

    /* -------------------------------------------- [ generate workspace ] -- */
    {
      /* ----------------------------------------------------- [ project ] -- */
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

          /* generate file ids */

          struct XCode_file
          {
            char id[25];
            char ref[25];
            const char *name;
            const char *path;
            int type;
          };

          int file_count = sed_array_size(proj->files);

          struct XCode_file *xc_files = NULL;
          sed_array_create(xc_files, file_count);


          int k;

          for(k = 0; k < file_count; ++k)
          {
            struct XCode_file new_file;
            memset(&new_file, 0, sizeof(new_file));

            int l;

            for(l = 0; l < 24; ++l)
            {
              sed_xcode_id((char*)(new_file.id));
              sed_xcode_id((char*)(new_file.ref));
            }

            new_file.name = proj->files[k].name;
            new_file.type = proj->files[k].type;
            new_file.path = proj->files[k].path;

            sed_array_push(xc_files, new_file);
          }

          /* ----------------------------------- [ generate project file ] -- */

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

          for(k = 0; k < file_count; ++k)
          {
            if(xc_files[k].type == SED_FILE_TYPE_SOURCE)
            {
              sed_file_append("\t\t");
              sed_file_append(xc_files[k].id);
              sed_file_append(" /* ");
              sed_file_append(xc_files[k].name);
              sed_file_append(" in Sources */ = ");
              sed_file_append("{isa = PBXBuildFile; fileRef = ");
              sed_file_append(xc_files[k].ref);
              sed_file_append(" /* ");
              sed_file_append(xc_files[k].name);
              sed_file_append(" */; };");
              sed_file_append("\n");
            }
          }

          sed_file_append("/* End PBXBuildFile section */\n\n");

          /* file refs */

          sed_file_append("/* Begin PBXFileReference section */\n");
          for(k = 0; k < file_count; ++k)
          {
            sed_file_append("\t\t");
            sed_file_append(xc_files[k].id);
            sed_file_append(" /* ");
            sed_file_append(xc_files[k].name);
            sed_file_append(" */ = {isa = PBXFileReference; ");
            sed_file_append("lastKnownFileType = ");
            
            if(xc_files[k].type == SED_FILE_TYPE_SOURCE)
            {
              if(proj->lang < SED_LANG_CPP11)
              {
                sed_file_append("sourcecode.c.c; ");
              }
              else
              {
                sed_file_append("sourcecode.cpp.cpp; ");
              }
            }
            else
            {
              sed_file_append("text; ");
            }

            sed_file_append("name = \""); 
            sed_file_append(xc_files[k].name);
            sed_file_append("\"; ");
            sed_file_append("path = \"");
            sed_file_append(xc_files[k].path);
            sed_file_append("\"; sourceTree = \"<group>\"");
            sed_file_append("\n");
          }

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
          sed_array_destroy(xc_files);
        }
      }

      /* --------------------------------------------------- [ workspace ] -- */
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
