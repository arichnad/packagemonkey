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

#include "slack.h"

static void save_slack_desc(char * directory)
{
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char str[BLOCK_SIZE];
	FILE * fp;
	int i, max, n, ctr = 0, start;

	get_setting("project name", project_name);
	get_setting("description brief", description_brief);
	get_setting("description", description);

	sprintf(filename, "%s%c%s%cslack-desc",
			directory, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR, DIRECTORY_SEPARATOR);

	fp = fopen(filename, "w");
	if (!fp) return;

	fprintf(fp, "%s", "# HOW TO EDIT THIS FILE:\n");
	fprintf(fp, "%s", "# The \"handy ruler\" below makes it easier to " \
			"edit a package description.  Line\n");
	fprintf(fp, "%s", "# up the first '|' above the ':' following the " \
			"base package name, and the '|' on\n");
	fprintf(fp, "%s", "# the right side marks the last column you can " \
			"put a character in.  You must make\n");
	fprintf(fp, "%s", "# exactly 11 lines for the formatting to be " \
			"correct.  It's also customary to\n");
	fprintf(fp, "%s", "# leave one space after the ':'.\n\n");

	for (i = 0; i < strlen(project_name); i++) {
		fprintf(fp, "%s", " ");
	}
	fprintf(fp, "%s", "|-----handy-ruler");
	for (i = 0; i < 79 - strlen(project_name) - 17; i++) {
		fprintf(fp, "%s", "-");
	}
	fprintf(fp, "%s", "|\n");

	sprintf(str, "%s: %s (%s)\n",
			project_name, project_name, description_brief);

	if (strlen(str) >= 79) {
		str[78] = '\n';
		str[79] = 0;
		printf("WARNING: brief description exceeds 79 character line " \
			   "length within slack-desc\n");
	}
	fprintf(fp, "%s", str);
	
	fprintf(fp, "%s:\n", project_name);
	max = 79 - strlen(project_name) - 2;
	for (i = 0; i < 9; i++) {
		fprintf(fp, "%s:", project_name);
		n = 0;
		start = ctr;

		if (ctr < strlen(description)) {
			fprintf(fp, "%s", " ");
		}

		while ((n < max) &&
			   (ctr < strlen(description))) {			
			ctr++;
			n++;
		}

		if (n == max) {
			while ((ctr > 1) && (description[ctr] != ' ')) {
				ctr--;
				n--;
			}
			while ((ctr > 1) && (description[ctr] == ' ')) {
				ctr--;
				n--;
			}
			ctr++;
			n++;
			strncpy(str, &description[start], n);
			str[n] = 0;
			fprintf(fp, "%s", str);
			start += n;
			ctr++;
			n++;
		}
		else {
			if (start < strlen(description)) {
				fprintf(fp, "%s", &description[start]);
				start = strlen(description);
			}
		}
		
		fprintf(fp, "%s", "\n");
		
	}

	fclose(fp);
}

/* saves a script which creates a txz package */
static int save_slackbuild(char * directory)
{
	char script_filename[BLOCK_SIZE];
	char build_directory[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char tarball_base[BLOCK_SIZE];
	FILE * fp;

	get_setting("project name", project_name);
	get_setting("version", version);
	get_setting("release", release);

	/* Name of the script to build the package */
	sprintf(script_filename, "%s%cslack.sh",
			directory, DIRECTORY_SEPARATOR);

	/* Directory in which to build the package */
	sprintf(build_directory, "~%cslackbuild",
			DIRECTORY_SEPARATOR);

	/* Base name of the tarball */
	sprintf(tarball_base, "%s", "${APP}-${VERSION}-${RELEASE}.tar");

	/* Save the script */
	fp = fopen(script_filename, "w");
	if (!fp) return -1;

	fprintf(fp,"%s","#!/bin/bash\n\n");

	/* Create the build directory /tmp/buildpet/project-version */
	fprintf(fp, "APP=%s\n", project_name);
	fprintf(fp, "PREV_VERSION=%s\n", version);
	fprintf(fp, "VERSION=%s\n", version);
	fprintf(fp, "RELEASE=%s\n", release);
	fprintf(fp, "ARCH_TYPE=`uname -m`\n");
	fprintf(fp, "BUILDDIR=%s\n", build_directory);
	fprintf(fp, "%s", "CURRDIR=`pwd`\n");
	fprintf(fp, "%s", "PROJECTDIR=${BUILDDIR}/${APP}-${VERSION}-${RELEASE}\n");

	/* Alter the version numbers */
	script_version_numbers(fp, "slack");

	/* Make directories */
	fprintf(fp, "%s", "\n# Make directories within which the project " \
			"will be built\n");
	fprintf(fp, "%s -p ${BUILDDIR}\n", COMMAND_MKDIR);
	fprintf(fp, "%s -p ${PROJECTDIR}\n", COMMAND_MKDIR);

	/* Build */
	fprintf(fp, "%s", "\n# Build the project\n");
	fprintf(fp, "%s", "make clean\n");
	fprintf(fp, "%s", "make\n");
	if (is_library(project_name) == 0) {
		fprintf(fp, "%s", "make install -B DESTDIR=${PROJECTDIR} PREFIX=/usr\n");
	}
	else {
		fprintf(fp, "%s", "make instlib -B DESTDIR=${PROJECTDIR} PREFIX=/usr\n");
	}

	/* Copy the slack-desc and doinst.sh files */
	fprintf(fp,"%s","\n# Copy the slack-desc and doinst.sh files into " \
			"the build install directory\n");
	fprintf(fp, "%s ${PROJECTDIR}%cinstall\n",
			COMMAND_MKDIR, DIRECTORY_SEPARATOR);
	fprintf(fp, "%s ${CURRDIR}%c%s%cslack-desc ${PROJECTDIR}%cinstall\n",
			COMMAND_COPY, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);
	fprintf(fp, "%s ${CURRDIR}%c%s%cdoinst.sh ${PROJECTDIR}%cinstall\n",
			COMMAND_COPY, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);

	/* Compress the build directory */
	fprintf(fp, "%s", "\n# Compress the build directory\n");
	fprintf(fp, "%s", "cd ${BUILDDIR}\n");

	fprintf(fp, "tar -c -f %s .\n",
			tarball_base);
	fprintf(fp, "%s","sync\n");
	fprintf(fp, "xz %s\n", tarball_base);
	fprintf(fp, "%s","sync\n");
	fprintf(fp, "mv %s.xz ${CURRDIR}/%s/${APP}-${VERSION}-${ARCH_TYPE}-${RELEASE}.txz\n",
			tarball_base, SLACK_SUBDIR);

	/* Move back to the current directory and remove
	   the temporary directory */
	fprintf(fp, "%s", "cd ${CURRDIR}\n");
	fprintf(fp, "%s", "\n# Remove the temporary build directory\n");
	fprintf(fp, "%sr ${BUILDDIR}\n", COMMAND_DELETE);
	fclose(fp);

	sprintf(commandstr, "chmod +x %s", script_filename);
	return system(commandstr);
}

/* saves a doinst.sh script if none exists */
static int save_doinst(char * directory)
{
	char filename[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	FILE * fp;

	sprintf(filename, "%s%c%s%cdoinst.sh",
			directory, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR, DIRECTORY_SEPARATOR);

	if (file_exists(filename) != 0) return 0;

	fp = fopen(filename, "w");
	if (!fp) return -1;

	fprintf(fp,"%s","#!/bin/sh -e\n\n");
	fprintf(fp,"%s","# This script is run after installation.\n" \
			"# Any additional configuration goes here.\n");

	fclose(fp);

	sprintf(commandstr,"chmod +x %s", filename);
	return system(commandstr);
}

int save_slack()
{
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char subdir[BLOCK_SIZE];
	int retval = 0;

	get_setting("directory", directory);

	/* create the slackpackage subdirectory */
	sprintf(subdir, "%s%c%s",
			directory, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR);
	if (directory_exists(subdir) == 0) {
		sprintf(commandstr, "%s %s",
				COMMAND_MKDIR, subdir);
		retval = system(commandstr);
	}

	/* save a doinst script if none exists */
	save_doinst(directory);

	/* save the package description */
	save_slack_desc(directory);

	/* save the build script */
	save_slackbuild(directory);

	return retval;
}
