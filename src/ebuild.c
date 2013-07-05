/*
  packagemonkey - a package creation assistant
  Copyright (C) 2013  Bob Mottram <bob@robotics.uk.to>

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
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char license[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char depends[BLOCK_SIZE];
	char * build_depends[MAX_FILES];
	int i, no_of_depends;

	get_setting("project name",project_name);
	get_setting("version",version);
	get_setting("release",release);
	get_setting("homepage",homepage);
	get_setting("license",license);
	get_setting("depends ebuild",depends);
	get_setting("description",description);

	no_of_depends =
		separate_files(depends,
					   build_depends,
					   MAX_FILES);

	sprintf(filename,"%s%c%s%c%s-%s-%s.ebuild",
			directory, DIRECTORY_SEPARATOR,
			EBUILD_SUBDIR, DIRECTORY_SEPARATOR,
			project_name, version, release);

	fp = fopen(filename,"w");
	if (!fp) return -1;

	fprintf(fp,"%s","# $Header: $\n\n");

	fprintf(fp,"%s","EAPI=4\n\n");

	fprintf(fp,"DESCRIPTION=\"%s\"\n",description);
	fprintf(fp,"HOMEPAGE=\"%s\"\n",homepage);
	fprintf(fp,"%s","SRC_URI=\"${PN}/${P}.tar.gz\"\n");

	fprintf(fp,"LICENSE=\"%s\"\n",license);
	fprintf(fp,"%s","SLOT=\"0\"\n");
	fprintf(fp,"%s","KEYWORDS=\"x86\"\n");

	fprintf(fp,"%s","RDEPEND=\"dev-libs/popt\"\n");
	fprintf(fp,"%s","DEPEND=\"${RDEPEND}");
	for (i = 0; i < no_of_depends; i++) {
		fprintf(fp,"\n    %s", build_depends[i]);
		free(build_depends[i]);
	}
	fprintf(fp,"%s","\"\n");

    fprintf(fp,"%s","\nsrc_configure() {\n");
    fprintf(fp,"%s","    econf --with-popt\n");
    fprintf(fp,"%s","}\n\n");

    fprintf(fp,"%s","src_install() {\n");
	if (is_library(project_name) == 0) {
		fprintf(fp,"%s","    emake DESTDIR=\"${D}\" install\n");
	}
	else {
		fprintf(fp,"%s","    emake DESTDIR=\"${D}\" instlib\n");
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
	fprintf(fp, "%s", "# rename the root directory " \
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
