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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "tests.h"
#include "settings.h"
#include "makefile.h"
#include "debian.h"
#include "rpm.h"
#include "help.h"

int main(int argc, char* argv[])
{
	int i;
	int working_directory_specified = 0;
	char directory[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char section[BLOCK_SIZE];
	char license[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char email_address[BLOCK_SIZE];

	if (argc <= 1) {
		show_help();
		return 0;
	}

	/* parse options */
	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i],"-h")==0) ||
			(strcmp(argv[i],"--help")==0)) {
			show_help();
			return 0;
		}
		if ((strcmp(argv[i],"-v")==0) ||
			(strcmp(argv[i],"--version")==0)) {
			printf("%s\n",VERSION);
			return 0;
		}
		/* run unit tests */
		if ((strcmp(argv[i],"-t")==0) ||
			(strcmp(argv[i],"--tests")==0)) {
			run_tests();
			return 0;
		}
		/* set the working directory */
		if ((strcmp(argv[i],"-d")==0) ||
			(strcmp(argv[i],"--dir")==0)) {
			i++;
			if (i < argc) {
				add_setting("directory",argv[i]);
				working_directory_specified = 1;
			}
			else {
				printf("No working directory given\n");
			}
		}
		/* description of the package */
		if ((strcmp(argv[i],"--desc")==0) ||
			(strcmp(argv[i],"--description")==0)) {
			i++;
			if (i < argc) {
				add_setting("description",argv[i]);
			}
			else {
				printf("No description given\n");
			}
		}
		/* brief description of the package */
		if (strcmp(argv[i],"--brief")==0) {
			i++;
			if (i < argc) {
				add_setting("description brief",argv[i]);
			}
			else {
				printf("No brief description given\n");
			}
		}
		/* email address */
		if ((strcmp(argv[i],"-e")==0) ||
			(strcmp(argv[i],"--email")==0)) {
			i++;
			if (i < argc) {
				add_setting("email",argv[i]);
			}
			else {
				printf("No email address given\n");
			}
		}
		/* binary file or script to be packaged */
		if ((strcmp(argv[i],"-b")==0) ||
			(strcmp(argv[i],"--binary")==0) ||
			(strcmp(argv[i],"--executable")==0)) {
			i++;
			if (i < argc) {
				add_setting("binary",argv[i]);
			}
			else {
				printf("No binary filename given\n");
			}
		}
		/* scripts to be packaged */
		if ((strcmp(argv[i],"-s")==0) ||
			(strcmp(argv[i],"--scripts")==0)) {
			i++;
			if (i < argc) {
				add_setting("scripts",argv[i]);
			}
			else {
				printf("No script filename given\n");
			}
		}
		/* homepage */
		if ((strcmp(argv[i],"--url")==0) ||
			(strcmp(argv[i],"--homepage")==0)) {
			i++;
			if (i < argc) {
				add_setting("homepage",argv[i]);
			}
			else {
				printf("No home page given\n");
			}
		}
		/* vcs browser */
		if ((strcmp(argv[i],"--vcsbrowse")==0) ||
			(strcmp(argv[i],"--vcsprowser")==0)) {
			i++;
			if (i < argc) {
				add_setting("vcs browser",argv[i]);
			}
			else {
				printf("No version control URL given\n");
			}
		}
		/* license */
		if ((strcmp(argv[i],"-l")==0) ||
			(strcmp(argv[i],"--license")==0)) {
			i++;
			if (i < argc) {
				add_setting("license",argv[i]);
			}
			else {
				printf("No license given\n");
			}
		}
		/* vcs repository */
		if (strcmp(argv[i],"--repository")==0) {
			i++;
			if (i < argc) {
				add_setting("vcs repository",argv[i]);
			}
			else {
				printf("No version control repository given\n");
			}
		}
		/* Debian section */
		if (strcmp(argv[i],"--section")==0) {
			i++;
			if (i < argc) {
				add_setting("section",argv[i]);
			}
			else {
				printf("No Debian section given\n");
			}
		}
		/* Project type */
		if (strcmp(argv[i],"--type")==0) {
			i++;
			if (i < argc) {
				add_setting("project type",argv[i]);
			}
			else {
				printf("No project type given\n");
			}
		}
		/* Debian version */
		if (strcmp(argv[i],"--debian")==0) {
			i++;
			if (i < argc) {
				if (strcmp(argv[i],"squeeze")==0) {
					add_setting("debian version","6");
				}
				else {
					if (strcmp(argv[i],"wheezy")==0) {
						add_setting("debian version","7");
					}
					else {
						add_setting("debian version",argv[i]);
					}
				}
			}
			else {
				printf("No Debian version number given\n");
			}
		}
		/* list the Debian sections */
		if (strcmp(argv[i],"--list-sections")==0) {
			debian_list_sections();
			return 0;
		}

	}

	/* check that a working directory was given */
	if (working_directory_specified != 0) {
		printf("No working directory specified.  Use " \
			   "the --dir option to set the directory " \
			   "of the project to be packaged.\n");
		return -1;
	}

	/* check the directory exists */
	get_setting("directory",directory);
	if ((strlen(directory) == 0) ||
		(directory_exists(directory) == 0)) {
		printf("Directory %s not found\n", directory);
		return -1;
	} 

	/* get the project name from the directory */
	get_subdirectory(directory,project_name);
	if (valid_project_name(project_name)==0) {
		printf("The project name, which is taken from the " \
			   "directory within which the project exists, " \
			   "must be all lower case with no " \
			   "non-alphanumeric values\n");
		return -1;
	}
	add_setting("project name",project_name);
	add_setting("version","0.10");

	/* check that a license is specified */
	get_setting("license",license);
	if (strlen(license) == 0) {
		printf("No license specified\n");
		return -1;
	} 
	if (valid_license(license)==0) {
		printf("%s is not a recognised license\n",license);
		return -1;
	}

	/* check that the Debian sections are valid */
	get_setting("section", section);
	if (strlen(section) > 0) {
		if (debian_valid_section(section) == 0) {
			return -1;
		}
	}

	/* check that the email address is valid */
	get_setting("email", email_address);
	if (strlen(email_address) == 0) {
		printf("Please specify an email address using the --email option\n");
		return -1;
	}
	if (valid_email(email_address) == 0) {
		printf("%s is not a valid email.  Use the format:\n\n  name <user@domain>\n\n", email_address);
		return -1;
	}

	/* check that a homepage has been given */
	get_setting("homepage", homepage);
	if (strlen(homepage) == 0) {
		printf("Please specify a project homepage URL using the --homepage option\n");
		return -1;
	}

	save_debian();

	return 0;
}

