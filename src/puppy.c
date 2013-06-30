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

/*saves the spec file into the puppypackage directory */
static int save_spec(char * directory)
{
	char spec_filename[BLOCK_SIZE];
	char depends[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char project_full_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char puppy_desktop_category[BLOCK_SIZE];
	char dir_size[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char repository_path[BLOCK_SIZE];
	char compiled_with_distro[BLOCK_SIZE];
	char compiled_with_distro_version[BLOCK_SIZE];
	char puppy_repository[BLOCK_SIZE];
	char free_desktop_categories[BLOCK_SIZE];
	char * separate_depends[MAX_FILES];
	FILE * fp;
	int i, no_of_depends;

	get_setting("depends puppy",depends);
	get_setting("project name",project_name);
	get_setting("project full name",project_full_name);
	get_setting("version",version);
	get_setting("release",release);
	get_setting("description brief",description);
	get_setting("homepage",homepage);

	sprintf(spec_filename,"%s%c%s-%s.pet.specs",
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR,
			project_name,version);

	/* convert from free desktop categories 
	   to puppy desktop categories */
	get_setting("categories",free_desktop_categories);
	free_desktop_to_puppy_desktop(free_desktop_categories,
								  puppy_desktop_category);

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
	fprintf(fp,"%s|",project_full_name);
	fprintf(fp,"%s|",version);
	fprintf(fp,"%s|",release);
	fprintf(fp,"%s|",puppy_desktop_category);
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


static int save_script(char * directory,
					   char * xpm_filename)
{
	char script_filename[BLOCK_SIZE];
	char build_directory[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char categories[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char puppy_desktop_categories[BLOCK_SIZE];
	char tarball_base[BLOCK_SIZE];
	char commandline[BLOCK_SIZE];
	char free_main_category[BLOCK_SIZE];
	char free_additional_category[BLOCK_SIZE];
	FILE * fp;

	get_setting("project name",project_name);
	get_setting("version",version);
	get_setting("release",release);
	get_setting("categories",categories);
	get_setting("commandline",commandline);

	/* name of the script to build the package */
	sprintf(script_filename,"%s%cpuppy.sh",
			directory, DIRECTORY_SEPARATOR);

	/* directory in which to build the package */
	sprintf(build_directory,"~%cpetbuild",
			DIRECTORY_SEPARATOR);

	/* base name of the tarball */
	sprintf(tarball_base,"%s","{APP}-${VERSION}-${RELEASE}.tar");

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
	fprintf(fp, "%s", "CURRDIR=`pwd`\n");
	fprintf(fp,"%s","PROJECTDIR=${BUILDDIR}/${APP}-${VERSION}-${RELEASE}\n");

	/* alter the version numbers */
	script_version_numbers(fp,"puppy");

	/* make directories */
	fprintf(fp,"%s","\n# Make directories within which the project will be built\n");
	fprintf(fp,"%s -p ${BUILDDIR}\n",COMMAND_MKDIR);
	fprintf(fp,"%s -p ${PROJECTDIR}\n",	COMMAND_MKDIR);

	/* build */
	fprintf(fp,"%s","\n# Build the project\n");
	fprintf(fp,"%s","make clean\n");
	fprintf(fp,"%s","make\n");
	fprintf(fp,"%s","make install -B DESTDIR=${PROJECTDIR}\n");

	if (strlen(commandline) == 0) {
		/* alter the category within the .desktop file */
		free_desktop_to_puppy_desktop(categories,
									  puppy_desktop_categories);
		parse_desktop_category(categories,
							   free_main_category,
							   free_additional_category);
		fprintf(fp,"%s","\n# Alter the desktop file categories\n");
		fprintf(fp,"sed -i \"s/Categories=%s;%s;/Categories=%s/g\" " \
				"${PROJECTDIR}/usr/share/applications/${APP}.desktop\n",
				free_main_category,
				free_additional_category,
				puppy_desktop_categories);
	}

	/* create puppy-specific directories */
	fprintf(fp,"%s","\n# create directories specific to puppy\n");
	fprintf(fp,"%s ${PROJECTDIR}/usr\n", COMMAND_MKDIR);
	fprintf(fp,"%s ${PROJECTDIR}/usr/local\n", COMMAND_MKDIR);
	fprintf(fp,"%s ${PROJECTDIR}/usr/local/bin\n", COMMAND_MKDIR);

	/* copy puppy specific files */
	fprintf(fp,"%s","\n# copy anything in /usr/bin into /usr/local/bin\n");
	fprintf(fp,"%s ${PROJECTDIR}/usr/bin/* ${PROJECTDIR}/usr/local/bin/\n", COMMAND_COPY);

	/* copy the spec file */
	fprintf(fp,"%s","\n# Copy the spec file into the build directory\n");
	fprintf(fp,"%s %s%c%s-%s.pet.specs ${PROJECTDIR}\n",
			COMMAND_COPY,
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR,
			project_name, version);

	/* copy the xpm mini icon */
	if (strlen(commandline) == 0) {
		fprintf(fp,"%s",
				"\n# Copy the XPM mini icon into the build directory\n");
		if (strlen(xpm_filename) > 0) {
			fprintf(fp,"%s %s ${PROJECTDIR}%c%s.xpm\n",
					COMMAND_COPY, xpm_filename,
					DIRECTORY_SEPARATOR,
					project_name);
		}
		else {
			fprintf(fp,"%s %s%c*.xpm ${PROJECTDIR}\n",
					COMMAND_COPY,
					directory, DIRECTORY_SEPARATOR);
		}
	}

	/* compress the build directory */
	fprintf(fp,"%s","\n# Compress the build directory\n");
	fprintf(fp,"%s","cd ${BUILDDIR}\n");
	fprintf(fp,"tar -c -f %s .\n",
			tarball_base);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"gzip %s\n", tarball_base);
	fprintf(fp,"mv %s.gz ${CURRDIR}/%s\n",
			tarball_base, PUPPY_SUBDIR);
	fprintf(fp,"cd ${CURRDIR}/%s\n",PUPPY_SUBDIR);

	/* convert to PET format */
	fprintf(fp,"%s","\n# Create the PET package\n");
	fprintf(fp,"MD5SUM=\"`md5sum %s.gz | cut -f 1 -d ' '`\"\n",
			tarball_base);
	fprintf(fp,"echo -n \"$MD5SUM\" >> %s.gz\n", tarball_base);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"mv -f %s.gz ${APP}-${VERSION}-${RELEASE}.pet\n",
			tarball_base);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"%s","cd ${CURRDIR}\n");
	fprintf(fp,"%s","\n# Remove the temporary build directory\n");
	fprintf(fp,"%sr ${BUILDDIR}\n", COMMAND_DELETE);
	fclose(fp);

	sprintf(commandstr,"chmod +x %s", script_filename);
	return system(commandstr);
}

/* searches for an XPM file within the install directory.
   If an XPM file is found this is coppied into the
   puppypackage directory as the mini icon for the menu */
static int copy_mini_icon(char * directory,
						  int no_of_binaries, char ** binaries,
						  char * xpm_filename)
{
	char commandline[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int i,ctr=0;

	/* if this is a commandline project
	   then there is no need to check for an icon */
	get_setting("commandline",commandline);
	if (strlen(commandline) > 0) return 0;

	for (i = 0; i < no_of_binaries; i++) {
		if (strstr(binaries[i],".xpm")!=NULL) {
			sprintf(xpm_filename,"%s%c%s",
					directory, DIRECTORY_SEPARATOR,
					binaries[i]);
			ctr++;
		}
	}

	if (ctr == 0) {
		printf("WARNING: No XPM mini icon " \
			   "was located within the install " \
			   "directory.\n");
		return -1;
	}

	if (ctr > 1) {
		printf("WARNING: More than one XPM files were " \
			   "located within the install directory.  " \
			   "To avoid any confusion none of these " \
			   "have been used as the Puppy menu mini icon.\n");
		return -2;
	}

	/* copy the xpm into the puppypackage directory */
	sprintf(commandstr,"%s %s %s%c%s",
			COMMAND_COPY,
			xpm_filename,
			directory, DIRECTORY_SEPARATOR,
			PUPPY_SUBDIR);
	return system(commandstr);
}

int save_puppy(int no_of_binaries, char ** binaries)
{
	char xpm_filename[BLOCK_SIZE];
	char directory[BLOCK_SIZE];
	char subdir[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int retval=0;

	xpm_filename[0] = 0;
	get_setting("directory",directory);

	sprintf(subdir,"%s%c%s",directory,DIRECTORY_SEPARATOR,PUPPY_SUBDIR);
	if (directory_exists(subdir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,subdir);
		retval = system(commandstr);
	}

	save_spec(directory);
	copy_mini_icon(directory,
				   no_of_binaries, binaries,
				   xpm_filename);
	save_script(directory, xpm_filename);
	return retval;
}
