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

#include "puppy.h"

/* convert a free desktop category into a puppy linux category */
static void get_category(char * puppy_category)
{
	char free_desktop_categories[BLOCK_SIZE];
	char main_category[BLOCK_SIZE];
	char additional_category[BLOCK_SIZE];

	/* Puppy Linux categories:

	   Desktop System Setup Utility Filesystem Graphic
	   Document Calculate Personal Network Internet
	   Multimedia Fun Help BuildingBlock Develop */

	get_setting("categories", free_desktop_categories);

	sprintf(puppy_category,"%s","Desktop");

	parse_desktop_category(free_desktop_categories,
						   main_category,
						   additional_category);

	if ((strcmp(main_category,"AudioVideo") == 0) ||
		(strcmp(main_category,"Audio") == 0) ||
		(strcmp(main_category,"Video") == 0)) {
		sprintf(puppy_category,"%s","Multimedia");
	}

	if (strcmp(main_category,"Development") == 0) {
		sprintf(puppy_category,"%s","Develop");
	}
	if (strcmp(main_category,"Education") == 0) {
		sprintf(puppy_category,"%s","Personal");
	}
	if (strcmp(main_category,"Game") == 0) {
		sprintf(puppy_category,"%s","Fun");
	}
	if (strcmp(main_category,"Graphics") == 0) {
		sprintf(puppy_category,"%s","Graphic");
	}
	if (strcmp(main_category,"Network") == 0) {
		sprintf(puppy_category,"%s","Network");
	}
	if (strcmp(main_category,"Office") == 0) {
		sprintf(puppy_category,"%s","Personal");
	}
	if (strcmp(main_category,"Science") == 0) {
		sprintf(puppy_category,"%s","Calculate");
	}
	if ((strcmp(main_category,"Settings") == 0) ||
		(strcmp(main_category,"System") == 0)) {
		sprintf(puppy_category,"%s","System");
	}
	if (strcmp(main_category,"Utility") == 0) {
		sprintf(puppy_category,"%s","Utility");
	}
}

/*saves the spec file into the puppypackage directory */
static int save_spec(char * directory)
{
	char spec_filename[BLOCK_SIZE];
	char depends[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char category[BLOCK_SIZE];
	char dir_size[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char repository_path[BLOCK_SIZE];
	char compiled_with_distro[BLOCK_SIZE];
	char compiled_with_distro_version[BLOCK_SIZE];
	char puppy_repository[BLOCK_SIZE];
	char * separate_depends[MAX_FILES];
	FILE * fp;
	int i, no_of_depends;

	get_category(category);

	sprintf(spec_filename,"%s%cpet.specs",
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR);

	get_setting("depends puppy",depends);
	get_setting("project name",project_name);
	get_setting("version",version);
	get_setting("release",release);
	get_setting("description brief",description);
	get_setting("depends puppy",depends);
	get_setting("homepage",homepage);

	sprintf(compiled_with_distro,"%s","ubuntu");
	sprintf(compiled_with_distro_version,"%s","precise");
	sprintf(puppy_repository,"%s","5");

	/* path within repository, e.g. pet_packages-5 */
	sprintf(repository_path,"%s","");

	fp = fopen(spec_filename,"w");
	if (!fp) return -1;

	directory_size(directory,dir_size);
	no_of_depends =
		separate_files(depends,separate_depends,MAX_FILES);

	fprintf(fp,"%s-%s-%s|",project_name,version,release);
	fprintf(fp,"%s|",project_name);
	fprintf(fp,"%s|",version);
	fprintf(fp,"%s|",release);
	fprintf(fp,"%s|",category);
	fprintf(fp,"%s|",dir_size);
	fprintf(fp,"%s|",repository_path);
	fprintf(fp,"%s-%s-%s.pet|",project_name,version,release);

	for (i = 0; i < no_of_depends; i++) {
		if (i > 0) {
			fprintf(fp,"%s",",");
		}
		fprintf(fp,"+%s",separate_depends[i]);
		free(separate_depends[i]);
	}

	fprintf(fp,"|%s|",description);
	fprintf(fp,"%s|%s|",
			compiled_with_distro,
			compiled_with_distro_version);
	fprintf(fp,"%s|\n",puppy_repository);

	fclose(fp);
	return 0;
}

static int save_script(char * directory)
{
	char script_filename[BLOCK_SIZE];
	char build_directory[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char tarball_base[BLOCK_SIZE];
	FILE * fp;

	get_setting("project name",project_name);
	get_setting("version",version);
	get_setting("release",release);

	/* name of the script to build the package */
	sprintf(script_filename,"%s%cpuppy.sh",
			directory, DIRECTORY_SEPARATOR);

	/* directory in which to build the package */
	sprintf(build_directory,"~%cpetbuild",
			DIRECTORY_SEPARATOR);

	/* base name of the tarball */
	sprintf(tarball_base,"%s%c${APP}-${VERSION}-${RELEASE}.tar",
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR);

	/* save the script */
	fp = fopen(script_filename,"w");
	if (!fp) return -1;

	fprintf(fp,"%s","#!/bin/bash\n\n");

	/* create the build directory /tmp/buildpet/project-version */
	fprintf(fp,"APP=%s\n",project_name);
	fprintf(fp,"PREV_VERSION=%s\n",version);
	fprintf(fp,"VERSION=%s\n",version);
	fprintf(fp,"RELEASE=%s\n",release);
	fprintf(fp,"BUILDDIR=%s\n",build_directory);
	fprintf(fp,"%s","PROJECTDIR=${BUILDDIR}/${APP}-${VERSION}-${RELEASE}\n");

	/* alter the version numbers */
	fprintf(fp, "%s", "\n# Update version numbers " \
			"automatically - so you don't have to\n");
	fprintf(fp, "%s", "sed -i 's/VERSION='" \
			"${PREV_VERSION}'/VERSION='${VERSION}'/g'" \
			" Makefile debian.sh rpm.sh arch.sh\n");
	fprintf(fp, "sed -i 's/Version: '${PREV_VERSION}'" \
			"/Version: '${VERSION}'/g' %s/${APP}.spec\n",
			RPM_SUBDIR);
	fprintf(fp, "sed -i 's/Release: '${RELEASE}" \
			"'/Release: '${RELEASE}'/g' %s/${APP}.spec\n",
			RPM_SUBDIR);
	fprintf(fp, "sed -i 's/pkgrel='${RELEASE}'/" \
			"pkgrel='${RELEASE}'/g' %s/PKGBUILD\n",
			ARCH_SUBDIR);
	fprintf(fp, "sed -i 's/pkgver='${PREV_VERSION}'/"	\
			"pkgver='${VERSION}'/g' %s/PKGBUILD\n",
			ARCH_SUBDIR);

	/* make directories */
	fprintf(fp,"%s","\n# Make directories within which the project will be built\n");
	fprintf(fp,"%s -p ${BUILDDIR}\n",COMMAND_MKDIR);
	fprintf(fp,"%s -p ${PROJECTDIR}\n",	COMMAND_MKDIR);

	/* build */
	fprintf(fp,"%s","\n# Build the project\n");
	fprintf(fp,"%s","make clean\n");
	fprintf(fp,"%s","make\n");
	fprintf(fp,"%s","make install DESTDIR=${PROJECTDIR}\n");

	/* copy the spec file */
	fprintf(fp,"%s","\n# Copy the spec file into the build directory\n");
	fprintf(fp,"%s %s%cpet.specs ${PROJECTDIR}\n",
			COMMAND_COPY,
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR);

	/* compress the build directory */
	fprintf(fp,"%s","\n# Compress the build directory\n");
	fprintf(fp,"tar -c -f %s ${PROJECTDIR}\n",
			tarball_base);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"gzip %s\n", tarball_base);

	/* convert to PET format */
	fprintf(fp,"%s","\n# Create the PET package\n");
	fprintf(fp,"MD5SUM=\"`md5sum %s.gz | cut -f 1 -d ' '`\"\n",
			tarball_base);
	fprintf(fp,"echo -n \"$MD5SUM\" >> %s.gz\n", tarball_base);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"mv -f %s.gz %s%c${APP}-${VERSION}-${RELEASE}.pet\n",
			tarball_base, PUPPY_SUBDIR,
			DIRECTORY_SEPARATOR);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"%s","\n# Remove the temporary build directory\n");
	fprintf(fp,"%sr ${BUILDDIR}\n", COMMAND_DELETE);
	fclose(fp);

	sprintf(commandstr,"chmod +x %s", script_filename);
	return system(commandstr);
}

int save_puppy()
{
	char directory[BLOCK_SIZE];
	char subdir[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int retval=0;

	get_setting("directory",directory);

	sprintf(subdir,"%s%c%s",directory,DIRECTORY_SEPARATOR,PUPPY_SUBDIR);
	if (directory_exists(subdir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,subdir);
		retval = system(commandstr);
	}

	save_spec(directory);
	save_script(directory);
	return retval;
}
