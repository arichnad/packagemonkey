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
	FILE * fp;

	sprintf(spec_filename,"%s%cpet.specs",
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR);
	fp = fopen(spec_filename,"w");
	if (!fp) return -1;



	fclose(fp);
	return 0;
}

static int save_script(char * directory)
{
	char script_filename[BLOCK_SIZE];
	char build_directory[BLOCK_SIZE];
	char build_project_directory[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char tarball_base[BLOCK_SIZE];
	FILE * fp;

	get_setting("project name",project_name);
	get_setting("version",version);

	/* name of the script to build the package */
	sprintf(script_filename,"%s%cpuppy.sh",
			directory, DIRECTORY_SEPARATOR);

	/* directory in which to build the package */
	sprintf(build_directory,"%s%cbuildpet",
			TEMP_DIRECTORY, DIRECTORY_SEPARATOR);

	/* directory in which the project files will be installed */
	sprintf(build_project_directory,"%s%c%s-%s",
		    build_directory, DIRECTORY_SEPARATOR,
			project_name, version);

	/* base name of the tarball */
	sprintf(tarball_base,"%s%c%s-%s.tar",
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR,
			project_name, version);

	/* save the script */
	fp = fopen(script_filename,"w");
	if (!fp) return -1;

	fprintf(fp,"%s","#!/bin/bash\n\n");

	/* create the build directory /tmp/buildpet/project-version */
	fprintf(fp,"%s -p %s\n",COMMAND_MKDIR, build_directory);
	fprintf(fp,"%s -p %s\n",
			COMMAND_MKDIR, build_project_directory);

	/* build */
	fprintf(fp,"%s","\n# Build the project\n");
	fprintf(fp,"%s","make clean\n");
	fprintf(fp,"%s","make\n");
	fprintf(fp,"make install DESTDIR=%s\n",
			build_project_directory);

	/* copy the spec file */
	fprintf(fp,"%s","\n# Copy the spec file into the build directory\n");
	fprintf(fp,"%s %s%cpet.specs %s\n",
			COMMAND_COPY,
			PUPPY_SUBDIR, DIRECTORY_SEPARATOR,
			build_project_directory);

	/* compress the build directory */
	fprintf(fp,"%s","\n# Compress the build directory\n");
	fprintf(fp,"tar -c -f %s %s\n",
			tarball_base,
			build_project_directory); 
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"gzip %s\n", tarball_base);

	/* convert to PET format */
	fprintf(fp,"%s","\n# Create the PET package\n");
	fprintf(fp,"%s","FULLSIZE=\"`stat --format=%s ${TARBALL}`\"\n");

	fprintf(fp,"MD5SUM=\"`md5sum %s.gz | cut -f 1 -d ' '`\"\n",
			tarball_base);
	fprintf(fp,"echo -n \"$MD5SUM\" >> %s.gz\n", tarball_base);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"mv -f %s.gz %s%c%s-%s.pet\n",
			tarball_base, PUPPY_SUBDIR,
			DIRECTORY_SEPARATOR,
			project_name, version);
	fprintf(fp,"%s","sync\n");
	fprintf(fp,"%s","\n# Remove the temporary build directory\n");
	fprintf(fp,"%sr %s\n", COMMAND_DELETE, build_directory);
	fclose(fp);

	return 0;
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
