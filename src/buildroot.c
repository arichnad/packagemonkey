/*
  See
  http://buildroot.uclibc.org/downloads/manual/manual.html#adding-packages

  packagemonkey - a package creation assistant
  Copyright (C) 2014  Bob Mottram <bob@robotics.uk.to>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "buildroot.h"

/**
 * @brief Returns the Make constant used to reference a particular package
 * @param project_name Name of the project
 * @param name_constant Returned Make constant for the project
 * @return 0 on success
 */
static int get_project_name_constant(char * project_name, char * name_constant)
{
    char project_name_upper[BLOCK_SIZE];
    int i;

    if (strlen(project_name) == 0) return -1;

    string_to_upper(project_name, project_name_upper);
    sprintf(name_constant, "BR2_PACKAGE_%s", project_name_upper);

    /* replace spaces with underscores */
    for (i = 0; i < strlen(project_name_upper); i++) {
        if (project_name_upper[i]==' ') {
            project_name_upper[i]='_';
        }
    }
    return 0;
}
/**
 * @brief Save the Config.in file
 * @param directory The directory to save in
 * @return 0 on success
 */
static int save_config_in(char * directory)
{
    FILE * fp;
    char filename[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char project_name_constant[BLOCK_SIZE];
    char selects_constant[BLOCK_SIZE];
    char depends_constant[BLOCK_SIZE];
    char depends[BLOCK_SIZE];
    char selects[BLOCK_SIZE];
    char homepage[BLOCK_SIZE];
    char description_brief[BLOCK_SIZE];
    char * packages[MAX_FILES];
    int i,n;

    get_setting("project name", project_name);
    if (strlen(project_name) == 0) return -1;
    get_setting("brselect", selects);
    get_setting("brdepends", depends);
    get_setting("description brief", description_brief);
    get_setting("homepage", homepage);

    sprintf(filename,"%s%cConfig.in",directory,DIRECTORY_SEPARATOR);
    fp = fopen(filename,"w");
    if (!fp) {
        printf("Unable to write to %s\n", filename);
        return -1;
    }

    get_project_name_constant(project_name, project_name_constant);

    fprintf(fp,"%s\n", project_name_constant);
    fprintf(fp,"\tbool \"%s\"\n", project_name);

    if (strlen(selects) > 0) {
        n = separate_files(selects, packages, MAX_FILES);
        for (i = 0; i < n; i++) {
            get_project_name_constant(packages[i], selects_constant);
            fprintf(fp,"\tselect %s\n", selects_constant);
        }

        /* free allocated memory */
        for (i = 0; i < n; i++) {
            free(packages[i]);
        }
    }

    if (strlen(depends)>0) {
        n = separate_files(depends, packages, MAX_FILES);
        for (i = 0; i < n; i++) {
            get_project_name_constant(packages[i], depends_constant);
            fprintf(fp,"\tdepends on %s\n", depends_constant);
        }
        /* free allocated memory */
        for (i = 0; i < n; i++) {
            free(packages[i]);
        }
    }

    fprintf(fp,"\t%s\n","help");

    fprintf(fp,"\t  %s\n",description_brief);
    fprintf(fp,"\t  %s\n",homepage);

    fclose(fp);


    return 0;
}

/**
 * @brief Save the makefile
 * @param directory The directory to save in
 * @return 0 on success
 */
static int save_makefile(char * directory)
{
    FILE * fp;
    int i, n;
    char filename[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char version[BLOCK_SIZE];
    char license[BLOCK_SIZE];
    char license_upper[BLOCK_SIZE];
    char project_name_upper[BLOCK_SIZE];
    char download_site[BLOCK_SIZE];
    char br_dependencies[BLOCK_SIZE];
    char commit[BLOCK_SIZE];
    char * packages[MAX_FILES];

    get_setting("project name", project_name);
    if (strlen(project_name) == 0) return -1;
    get_setting("version", version);
    get_setting("downloadsite", download_site);
    get_setting("brdependencies", br_dependencies);
    get_setting("license", license);
    get_setting("commit", commit);

    sprintf(filename,"%s%c%s.mk",directory,DIRECTORY_SEPARATOR,project_name);
    fp = fopen(filename,"w");
    if (!fp) {
        printf("Unable to write to %s\n", filename);
        return -1;
    }

    fprintf(fp,"%s\n","###############################################" \
            "##################################");
    fprintf(fp,"%s\n","#");
    fprintf(fp,"# %s\n", project_name);
    fprintf(fp,"%s\n","#");
    fprintf(fp,"%s\n","###############################################" \
            "##################################");

    string_to_upper(project_name, project_name_upper);
    string_to_upper(license, license_upper);

    fprintf(fp,"%s_SITE = %s\n",
            project_name_upper, download_site);

    if (strlen(commit) > 0) {
        fprintf(fp,"%s_VERSION = %s\n", project_name_upper, commit);
        if ((strstr(download_site,"git:")) ||
            (strstr(download_site,"github"))) {
            fprintf(fp,"%s\n","MYPKG_SITE_METHOD = git");
        }
        else if (strstr(download_site,"svn:")) {
            fprintf(fp,"%s\n","MYPKG_SITE_METHOD = svn");
        }
        else if (strstr(download_site,"launchpad")) {
            fprintf(fp,"%s\n","MYPKG_SITE_METHOD = bzr");
        }
        else if (strstr(download_site,"cvs:")) {
            fprintf(fp,"%s\n","MYPKG_SITE_METHOD = cvs");
        }
    }
    else {
        fprintf(fp,"%s_VERSION = %s\n",project_name_upper,version);
        fprintf(fp,"%s_SOURCE = %s-$(%s_VERSION).tar.gz\n",
                project_name_upper, project_name, project_name_upper);
    }

    fprintf(fp,"%s_LICENSE = %s\n",
            project_name_upper, license_upper);
    fprintf(fp,"%s_LICENSE_FILES = LICENSE\n",
            project_name_upper);

    /*fprintf(fp,"%s_INSTALL_STAGING = YES\n",project_name_upper);*/

    if (strlen(br_dependencies) > 0) {
        n = separate_files(br_dependencies, packages, MAX_FILES);
        if (n > 0) {
            fprintf(fp,"%s_DEPENDENCIES = ",
                    project_name_upper);
            for (i = 0; i < n; i++) {
                fprintf(fp,"%s", packages[i]);
                if (i < n-1) {
                    fprintf(fp,"\t %c\n",'/');
                }
            }
            fprintf(fp,"%s","\n");
        }
        /* free allocated memory */
        for (i = 0; i < n; i++) {
            free(packages[i]);
        }
    }

    fprintf(fp,"%s\n","$(eval $(generic-package))");

    fclose(fp);
    return 0;
}

int save_buildroot()
{
    char buildrootdir[BLOCK_SIZE];
    char buildrootpackage[BLOCK_SIZE];
    char buildrootproject[BLOCK_SIZE];
    char directory[BLOCK_SIZE];
    char commandstr[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    int retval=0;

    get_setting("project name", project_name);
    if (strlen(project_name) == 0) return -1;

    /* create the buildroot directory */
    get_setting("directory", directory);
    sprintf(buildrootdir,"%s%c%s",
            directory, DIRECTORY_SEPARATOR, BUILDROOT_SUBDIR);
    if (directory_exists(buildrootdir)==0) {
        sprintf(commandstr,"%s %s",COMMAND_MKDIR,buildrootdir);
        retval = system(commandstr);
    }

    sprintf(buildrootpackage,"%s%cpackage",
            buildrootdir, DIRECTORY_SEPARATOR);
    if (directory_exists(buildrootpackage)==0) {
        sprintf(commandstr,"%s %s",COMMAND_MKDIR,buildrootpackage);
        retval = system(commandstr);
    }

    sprintf(buildrootproject,"%s%c%s",
            buildrootpackage, DIRECTORY_SEPARATOR,project_name);
    if (directory_exists(buildrootproject)==0) {
        sprintf(commandstr,"%s %s",COMMAND_MKDIR,buildrootproject);
        retval = system(commandstr);
    }

    save_config_in(buildrootproject);
    save_makefile(buildrootproject);

    return retval;
}
