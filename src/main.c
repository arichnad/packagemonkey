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
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "globals.h"
#include "tests.h"
#include "settings.h"
#include "makefile.h"
#include "license.h"
#include "debian.h"
#include "desktop.h"
#include "rpm.h"
#include "directory.h"
#include "arch.h"
#include "puppy.h"
#include "ebuild.h"
#include "help.h"

/* deallocates memory used for filenames */
void free_filenames(char ** filenames, int no_of_filenames)
{
	int i;
	if (no_of_filenames > 0) {
		for (i = 0; i < MAX_FILES; i++) {
			free(filenames[i]);
			filenames[i]=0;
		}
	}
}

int validate_description(char * description)
{
	int retval = valid_description(description);
	switch(retval) {
	case -1: {
		printf("A description of the project should be " \
			   "given using the --desc option\n");
		return -1;
	}
	case -2: {
		printf("The description should not begin with an " \
			   "indefinite or definite article such as \"A\", " \
			   "\"An\", \"This\" or \"The\"\n");
		return -1;
	}
	case -3: {
		printf("The description should not begin with a " \
			   "space or tab character\n");
		return -1;
	}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int i, j, retval;
	int no_of_binaries = 0;
	int working_directory_specified = 0;
	char directory[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char project_full_name[BLOCK_SIZE];
	char project_type[BLOCK_SIZE];
	char project_version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char section[BLOCK_SIZE];
	char license[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char categories[BLOCK_SIZE];
	char email_address[BLOCK_SIZE];
	char main_category[BLOCK_SIZE];
	char additional_category[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char str[BLOCK_SIZE*4];
	char test_filename[BLOCK_SIZE];
	char * binaries[MAX_FILES];

	if (argc <= 1) {
		show_help();
		return 0;
	}

	sprintf(project_version,"%s","0.10");
	add_setting("commandline","");
	add_setting("version",project_version);
	add_setting("binaries","install");
	add_setting("compile","");
	add_setting("release","1");
	add_setting("depends deb","");
	add_setting("depends rpm","");
	add_setting("depends arch","");
	add_setting("depends puppy","");
	add_setting("depends ebuild","");
	add_setting("build deb","");
	add_setting("build rpm","");
	add_setting("build arch","");
	add_setting("source package", "");
	add_setting("project full name", "");
	add_setting("mime types","");

	/* parse options */
	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i],"-h")==0) ||
			(strcmp(argv[i],"--help")==0)) {
			show_help();
			return 0;
		}
		if ((strcmp(argv[i],"-v")==0) ||
			(strcmp(argv[i],"--version")==0)) {
			if (argc == 2) {
				/* only one option */
				printf("%s\n",VERSION);
				return 0;
			}
			/* multiple options */
			i++;
			if (i < argc) {
				sprintf(project_version, "%s", argv[i]);
				add_setting("version", project_version);
			}
			else {
				printf("No version number given\n");
			}
		}
		/* release number */
		if ((strcmp(argv[i],"-r")==0) ||
			(strcmp(argv[i],"--release")==0)) {
			i++;
			if (i < argc) {
				sprintf(release, "%s", argv[i]);
				add_setting("release", release);
			}
			else {
				printf("No release number given\n");
			}
		}
		/* mime types */
		if ((strcmp(argv[i],"-m")==0) ||
			(strcmp(argv[i],"--mime")==0) ||
			(strcmp(argv[i],"--mimetype")==0) ||
			(strcmp(argv[i],"--mimetypes")==0)) {
			i++;
			if (i < argc) {
				add_setting("mime types", argv[i]);
			}
			else {
				printf("No Mime types given\n");
			}
		}
		/* Full project name */
		if ((strcmp(argv[i],"--name")==0) ||
			(strcmp(argv[i],"-n")==0)) {
			i++;
			if (i < argc) {
				add_setting("project full name", argv[i]);
			}
			else {
				printf("No full project name given\n");
			}
		}
		/* run unit tests */
		if ((strcmp(argv[i],"-t")==0) ||
			(strcmp(argv[i],"--tests")==0)) {
			run_tests();
			return 0;
		}
		/* Is this a commandline project? -
		   i.e. no desktop icon is needed */
		if ((strcmp(argv[i],"-c")==0) ||
			(strcmp(argv[i],"--cmd")==0) ||
			(strcmp(argv[i],"--commandline")==0)) {
			add_setting("commandline","1");
		}
		/* set the working directory */
		if ((strcmp(argv[i],"-d")==0) ||
			(strcmp(argv[i],"--dir")==0)) {
			i++;
			if (i < argc) {
				if ((strlen(argv[i])==0) ||
					(strcmp(argv[i],".")==0)) {
					char cwd[BLOCK_SIZE];
					if (getcwd(cwd, sizeof(cwd)) != NULL) {
						printf("Directory: %s\n", cwd);
						add_setting("directory",cwd);
					}
					else {
						printf("No working directory given\n");
						return -1;
					}
				}
				else {
					add_setting("directory",argv[i]);
				}
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
				if (strlen(argv[i]) > 66) {
					printf("The brief description should " \
						   "be less than 66 characters in " \
						   "length\n");
					return -1;
				}
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
		/* specify a directory containing binaries to be packaged */
		if ((strcmp(argv[i],"-b")==0) ||
			(strcmp(argv[i],"--binaries")==0) ||
			(strcmp(argv[i],"--executables")==0)) {
			i++;
			if (i < argc) {
				add_setting("binaries",argv[i]);
			}
			else {
				printf("No binaries directory given\n");
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
				/* convert to lower case */
				for (j = 0; j < strlen(argv[i]); j++) {
					argv[i][j] = tolower(argv[i][j]);
				}

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
		/* Set desktop file categories */
		if (strcmp(argv[i],"--categories")==0) {
			i++;
			if (i < argc) {
				add_setting("categories",argv[i]);
			}
			else {
				printf("No desktop categories given\n");
			}
		}
		/* RPM packages which the build requires */
		if (strcmp(argv[i],"--buildrpm")==0) {
			i++;
			if (i < argc) {
				add_setting("build rpm",argv[i]);
			}
			else {
				printf("No RPM packages given for the build\n");
			}
		}
		/* Arch packages which the build requires */
		if (strcmp(argv[i],"--buildarch")==0) {
			i++;
			if (i < argc) {
				add_setting("build arch",argv[i]);
			}
			else {
				printf("No Arch packages given for the build\n");
			}
		}
		/* deb packages which the build depends upon */
		if (strcmp(argv[i],"--builddeb")==0) {
			i++;
			if (i < argc) {
				add_setting("build deb",argv[i]);
			}
			else {
				printf("No deb packages given for the build\n");
			}
		}
		/* deb packages which this depends upon */
		if (strcmp(argv[i],"--dependsdeb")==0) {
			i++;
			if (i < argc) {
				add_setting("depends deb",argv[i]);
			}
			else {
				printf("No deb depends packages given\n");
			}
		}
		/* RPM packages which this depends upon */
		if (strcmp(argv[i],"--dependsrpm")==0) {
			i++;
			if (i < argc) {
				add_setting("depends rpm",argv[i]);
			}
			else {
				printf("No RPM depends packages given\n");
			}
		}
		/* Arch packages which this depends upon */
		if (strcmp(argv[i],"--dependsarch")==0) {
			i++;
			if (i < argc) {
				add_setting("depends arch",argv[i]);
			}
			else {
				printf("No Arch depends packages given\n");
			}
		}
		/* Puppy packages which this depends upon */
		if (strcmp(argv[i],"--dependspuppy")==0) {
			i++;
			if (i < argc) {
				add_setting("depends puppy",argv[i]);
			}
			else {
				printf("No Puppy depends packages given\n");
			}
		}
		/* Ebuild packages which this depends upon */
		if (strcmp(argv[i],"--dependsebuild")==0) {
			i++;
			if (i < argc) {
				add_setting("depends ebuild",argv[i]);
			}
			else {
				printf("No ebuild depends packages given\n");
			}
		}
		/* Additional arguments for the compiler */
		if (strcmp(argv[i],"--compile")==0) {
			i++;
			if (i < argc) {
				add_setting("compile",argv[i]);
			}
			else {
				printf("No compile arguments given\n");
			}
		}
		/* URL for the compressed source code */
		if ((strcmp(argv[i],"--src")==0) ||
			(strcmp(argv[i],"--source")==0)) {
			i++;
			if (i < argc) {
				add_setting("source package",argv[i]);
			}
			else {
				printf("No source package given\n");
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
		/* list the Desktop file categories */
		if (strcmp(argv[i],"--list-desktop")==0) {
			show_categories();
			return 0;
		}

	}

	/* check that a working directory was given */
	if (working_directory_specified == 0) {
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
	add_setting("version",project_version);

	/* Check that a full project name was specified.
	   If not the use the name from the project directory */
	get_setting("project full name",project_full_name);
	if (strlen(project_full_name)==0) {
		sprintf(project_full_name,"%s",project_name);
		project_full_name[0] = toupper(project_full_name[0]);
		add_setting("project full name",project_full_name);
	}

	/* check that a license is specified */
	get_setting("license",license);
	if (strlen(license) == 0) {
		printf("No license specified\n");
		return -1;
	} 
	if (valid_license(license)==0) {
		printf("%s is not a recognised license.\n",license);
		return -1;
	}
	/* convert license to upper case */
	if (strcmp(license,"mozilla")!=0) {
		for (i = 0; i < strlen(license); i++) {
			license[i] = toupper(license[i]);
		}
		add_setting("license",license);
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
		printf("Please specify an email address using " \
			   "the --email option\n");
		return -1;
	}
	if (valid_email(email_address) == 0) {
		printf("%s is not a valid email.  Use the " \
			   "format:\n\n  name <user@domain_name>\n\n",
			   email_address);
		return -1;
	}
	if (valid_gpg(email_address) == 0) {
		printf("WARNING: The email address %s does not " \
			   "appear to be a GPG key.\n", email_address);
		printf("Run 'gpg --list-keys' to see the keys on " \
			   "your system.\n");
		printf("You will not be able to sign Debian " \
			   "packages without a GPG key.\n");
	}

	/* check that a homepage has been given */
	get_setting("homepage", homepage);
	if (strlen(homepage) == 0) {
		printf("Please specify a project homepage URL " \
			   "using the --homepage option\n");
		return -1;
	}

	get_setting("categories", categories);
	parse_desktop_category(categories, main_category,
						   additional_category);
	if (strlen(main_category)==0) {
		printf("No desktop file main category defined.  " \
			   "Use the --categories option to do this.\n\n");
		show_categories_main();
		return -1;
	}
	if (valid_main_category(main_category) == -1) {
		printf("%s is not a valid main category\n\n",
			   main_category);
		show_categories_main();
		return -1;
	}
	if (strlen(additional_category)==0) {
		printf("An additional desktop/group category " \
			   "should be specified using the " \
			   "--categories option\n");
		show_categories_additional();
		return -1;		
	}
	if (valid_additional_category(additional_category) == -1) {
		printf("%s is not a valid additional category\n\n",
			   additional_category);
		show_categories_additional();
		return -1;
	}

	/* detect the type of project */
	add_setting("project type", "");
	detect_project_type(directory, project_type);
	if (strlen(project_type) > 0) {
		add_setting("project type", project_type);
		printf("Project Type: %s\n", project_type);
	}

	/* check that a brief description as given */
	get_setting("description brief",description_brief);
	if (validate_description(description_brief) < 0) {
		printf("Brief description: %s\n", description_brief);
		return -1;
	}

	/* check that a description as given */
	get_setting("description",description);
	if (validate_description(description) < 0) {
		printf("Description: %s\n", description);
		return -1;
	}

	printf("Project Version: %s\n", project_version);

	/* get the binary files to be packaged */
	get_setting("binaries",str);
	if ((strlen(str) > 0) && (directory_exists(str) != 0)) {
		/* allocate memory for filenames */
		for (i = 0; i < MAX_FILES; i++) {
			binaries[i] = (char*)malloc(BLOCK_SIZE);
		}

		retval =
			files_in_directory(str, ".\\.$",
							   WS_DOTFILES|WS_DEFAULT|WS_MATCHDIRS,
							   binaries, &no_of_binaries,
							   MAX_FILES);
		switch(retval) {
		case WALK_BADIO: {
			fprintf(stderr,"Directory '%s' Error %d: %s\n", str,errno,strerror(errno));
			break;
		}
		case WALK_NAMETOOLONG: {
			fprintf(stderr,"Directory name '%s' is too long\n", str);
			break;
		}
		case WALK_BADPATTERN: {
			fprintf(stderr,"Bad directory '%s'\n", str);
			break;
		}
		}

		for (i = 0; i < no_of_binaries; i++) {
			printf("Binary file: %s\n",binaries[i]);
			if (binaries[i][0] != DIRECTORY_SEPARATOR) {
				sprintf(test_filename,"%s%c%s",directory,
						DIRECTORY_SEPARATOR,
						binaries[i]);
			}
			else {
				sprintf(test_filename,"%s",binaries[i]);
			}
			if (file_exists(test_filename) == 0) {
				printf("File not found\n");
				/* deallocate memory */
				free_filenames(binaries,no_of_binaries);
				return -1;
			}
		}
	}

    save_configure(directory);
	save_license(directory);
	save_debian(no_of_binaries,binaries);
	save_desktop();
	save_makefile(no_of_binaries,binaries);
	save_rpm(no_of_binaries,binaries);
	save_arch();
	save_puppy(no_of_binaries,binaries);
	save_ebuild();

	/* free memory */
	free_filenames(binaries,no_of_binaries);

	return 0;
}

