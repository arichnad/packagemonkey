/*
  packagemonkey - a package creation assistant
  Copyright (C) 2013-2015  Bob Mottram <bob@robotics.uk.to>

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

#include "ebuild.h"

/* save the ebuild file */
int save_file(char * directory)
{
    FILE * fp;
    char filename[BLOCK_SIZE];
    char description[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char version[BLOCK_SIZE];
    char release[BLOCK_SIZE];
    char homepage[BLOCK_SIZE];
    char license[BLOCK_SIZE];
    char commandstr[BLOCK_SIZE];
    char depends[BLOCK_SIZE];
    char build_depends[BLOCK_SIZE];
    char repository[BLOCK_SIZE];
    char installdir[BLOCK_SIZE];
    char python_version[BLOCK_SIZE];
    char python_requires[BLOCK_SIZE];
    char * build_depends_files[MAX_FILES];
    char * depends_files[MAX_FILES];
    char * python_requires_packages[MAX_FILES];
    int i, no_of_depends, no_of_build_depends, use_git = 0;
    int no_of_python_requires;

    get_setting("project name", project_name);
    get_setting("project type", project_type);
    get_setting("version", version);
    get_setting("release", release);
    get_setting("homepage", homepage);
    get_setting("license", license);
    get_setting("build ebuild", build_depends);
    get_setting("depends ebuild", depends);
    get_setting("description", description);
    get_setting("vcs repository", repository);
    get_setting("python version", python_version);
    get_setting("python requires", python_requires);

    /* decide whether to use a git repository */
    sprintf(installdir, "%s%cinstall",
            directory, DIRECTORY_SEPARATOR);
    /* if an install subdirectory exists then don't use
       a git repository, since it probably doesn't
       contain binaries */
    if (directory_exists(installdir) == 0) {
        if (strlen(repository) > 0) {
            if (strstr(repository,"git") != NULL) {
                use_git = 1;
            }
        }
    }

    no_of_depends =
        separate_files(depends,
                       depends_files,
                       MAX_FILES);

    no_of_build_depends =
        separate_files(build_depends,
                       build_depends_files,
                       MAX_FILES);

    sprintf(filename, "%s%c%s%c%s-%s-%s.ebuild",
            directory, DIRECTORY_SEPARATOR,
            EBUILD_SUBDIR, DIRECTORY_SEPARATOR,
            project_name, version, release);

    fp = fopen(filename, "w");
    if (!fp) return -1;

    fprintf(fp,"%s","# $Header: $\n\n");

    fprintf(fp,"%s","EAPI=5\n\n");

    /* inherit */
    if (use_git == 1) {
        fprintf(fp,"%s","inherit git-2");
        if (strcmp(project_type,"py")==0) {
            fprintf(fp,"%s"," python-r1");
        }
        fprintf(fp,"%s","\n\n");
    }
    else {
        if (strcmp(project_type,"py")==0) {
            fprintf(fp,"%s","inherit python-r1\n\n");
        }
    }

    /* python specific stuff */
    if (strcmp(project_type,"py")==0) {
        if (strlen(python_version) > 0) {
            if (python_version[0] == '2') {
                fprintf(fp, "%s", "PYTHON_COMPAT=( python2_7 )\n");
            }
        }
        if (strlen(python_requires)>0) {
            no_of_python_requires =
                separate_files(python_requires,
                               python_requires_packages,
                               MAX_FILES);
            if (no_of_python_requires > 0) {
                fprintf(fp, "PYTHON_REQ_USE=\"%s",
                        python_requires_packages[0]);
                for (i = 1; i < no_of_python_requires; i++) {
                    fprintf(fp, "\n    %s",
                            python_requires_packages[i]);
                    free(python_requires_packages[i]);
                }
                fprintf(fp, "%s", "\"\n");
            }
        }
        fprintf(fp,"%s","REQUIRED_USE=\"${PYTHON_REQUIRED_USE}\"\n");
    }

    fprintf(fp,"DESCRIPTION=\"%s\"\n", description);
    fprintf(fp,"HOMEPAGE=\"%s\"\n", homepage);

    if (use_git == 0) {
        /* use the compressed source code */
        fprintf(fp, "%s", "SRC_URI=\"${PN}/${P}.tar.gz\"\n");
    }
    else {
        /* use a git repository */
        fprintf(fp, "EGIT_REPO_URI=\"%s\"\n", repository);
    }

    fprintf(fp, "LICENSE=\"%s\"\n", license);
    fprintf(fp, "%s", "SLOT=\"0\"\n");
    fprintf(fp, "%s", "KEYWORDS=\"x86\"\n");

    fprintf(fp, "%s", "DEPEND=\"dev-libs/popt");
    for (i = 0; i < no_of_build_depends; i++) {
        fprintf(fp, "\n    %s", build_depends_files[i]);
        free(build_depends_files[i]);
    }
    fprintf(fp,"%s","\"\n");

    fprintf(fp, "%s", "RDEPEND=\"${DEPEND}");
    for (i = 0; i < no_of_depends; i++) {
        fprintf(fp, "\n    %s", depends_files[i]);
        free(depends_files[i]);
    }
    fprintf(fp, "%s", "\"\n");

    fprintf(fp, "%s", "\nsrc_configure() {\n");
    fprintf(fp, "%s", "    econf --with-popt\n");
    fprintf(fp, "%s", "}\n\n");

    fprintf(fp, "%s", "src_compile() { :; }\n\n");

    fprintf(fp, "%s", "src_install() {\n");
    if (is_library(project_name) == 0) {
        fprintf(fp,"%s","    emake DESTDIR=\"${D}\" PREFIX=\"/usr\" install\n");
    }
    else {
        fprintf(fp,"%s","    emake DESTDIR=\"${D}\" PREFIX=\"/usr\" instlib\n");
    }
    fprintf(fp,"%s",
            "    # Install README and (Debian) changelog\n");
    fprintf(fp,"    dodoc README.md %s/changelog\n",
            DEB_SUBDIR);
    fprintf(fp,"%s","}\n");
    fclose(fp);

    /* make the ebuild executable,
       since it's just a bash script */
    sprintf(commandstr,"chmod +x %s", filename);
    return system(commandstr);
}

static int save_script(char * directory)
{
    FILE * fp;
    char filename[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char version[BLOCK_SIZE];
    char commandstr[BLOCK_SIZE];
    char release[BLOCK_SIZE];

    get_setting("project name", project_name);
    get_setting("version", version);
    get_setting("release",release);

    sprintf(filename,"%s%cebuild.sh",
            directory, DIRECTORY_SEPARATOR);

    fp = fopen(filename,"w");
    if (!fp) return -1;

    fprintf(fp, "%s", "#!/bin/bash\n\n");

    fprintf(fp, "APP=%s\n",project_name);
    fprintf(fp, "PREV_VERSION=%s\n", version);
    fprintf(fp, "VERSION=%s\n",version);
    fprintf(fp, "RELEASE=%s\n",release);
    fprintf(fp, "%s", "SOURCEDIR=.\n");
    fprintf(fp, "%s", "ARCH_TYPE=`uname -m`\n");
    fprintf(fp, "%s", "CURRDIR=`pwd`\n");
    fprintf(fp, "%s", "SOURCE=~/ebuild" \
            "/${APP}-${VERSION}.tar.gz\n\n");

    /* alter the version numbers */
    script_version_numbers(fp,"ebuild");

    fprintf(fp, "%s", "# create the source code in " \
            "the SOURCES directory\n");
    fprintf(fp, "%s", "make clean\n");
    fprintf(fp, "%s -p ~/ebuild\n", COMMAND_MKDIR);
    fprintf(fp, "%s", "rm -f ${SOURCE}\n");
    fprintf(fp, "%s", "mv ../${APP} ../${APP}-${VERSION}\n");
    fprintf(fp, "%s",
            "tar -cvzf ${SOURCE} ../${APP}-${VERSION} " \
            "--exclude-vcs\n");
    fprintf(fp, "%s", "\n# rename the root directory " \
            "without the version number\n");
    fprintf(fp, "%s", "mv ../${APP}-${VERSION} ../${APP}\n\n");

    fclose(fp);

    /* make the build script executable */
    sprintf(commandstr,"chmod +x %s", filename);
    return system(commandstr);
}

int save_ebuild()
{
    char directory[BLOCK_SIZE];
    char subdir[BLOCK_SIZE];
    char commandstr[BLOCK_SIZE];
    int retval=0;

    get_setting("directory",directory);

    sprintf(subdir,"%s%c%s",
            directory, DIRECTORY_SEPARATOR,
            EBUILD_SUBDIR);

    /* create the ebuildpackage directory */
    if (directory_exists(subdir)==0) {
        sprintf(commandstr,"%s %s",COMMAND_MKDIR,subdir);
        retval = system(commandstr);
    }

    save_file(directory);
    save_script(directory);
    return retval;
}
