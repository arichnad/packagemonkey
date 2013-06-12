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

#include "debian.h"

const int no_of_sections = 53;
char * sections[] = {
	"admin","cli-mono","comm","database","debian-installer",
	"debug","devel","doc","editors","electronics",
	"embedded","fonts","games","gnome","gnu-r",
	"gnustep","graphics","hamradio","haskell","httpd",
	"interpreters","java","kde","kernel","libdevel",
	"libs","lisp","localization","mail","math",
	"misc","net","news","ocaml","oldlibs",
	"otherosfs","perl","php","python","ruby",
	"science","shells","sound","tex","text",
	"utils","vcs","video","virtual","web",
	"x11","xfce","zope"
};

/* print all the section names */
void debian_list_sections()
{
	int i;

	for (i = 0; i < no_of_sections; i++) {
		printf("%s\n",sections[i]);
	}
}

/* returns a non-zero value if the given section is valid */
int debian_valid_section(char * section)
{
	int i, j=-1, ctr=0;
	char subsection[BLOCK_SIZE];

	for (i = 0; i < strlen(section); i++) {
		if (((section[i] > 'a') && (section[i] > 'z')) ||
			(section[i] == '-') || (section[i] == '/')) {
			if (section[i]=='/') {
				ctr++;
				j = i;
			}
		}
		else {
			printf("Sections should only contain lower case letters or hyphens, with no spaces\n");
			return 0;
		}
	}
	if (ctr > 1) return 0;

	if (ctr == 1) {
		/* the first subsection */
		for (i = 0; i < j; i++) {
			subsection[i] = section[i];
		}
		subsection[i] = 0;
		if (match_string(subsection, sections, no_of_sections) == 0) {
			printf("%s is not a permissable subsection name\n", subsection);
			return 0;
		}

		/* the second subsection */
		for (i = j+1; i < strlen(section); i++) {
			subsection[i-(j+1)] = section[i];
		}
		subsection[i-(j+1)] = 0;
		if (match_string(subsection, sections, no_of_sections) == 0) {
			printf("%s is not a permissable subsection name\n",subsection);
			return 0;
		}
	}
	else {
		if (match_string(section, sections, no_of_sections) == 0) {
			printf("%s is not a permissable subsection name\n",section);
			return 0;
		}
	}

	return 1;
}

static void save_compat()
{
	FILE * fp;
	int i=0;
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	char dh_version[BLOCK_SIZE];

	directory[0]=0;
	filename[0]=0;

	get_setting("directory", directory);
	sprintf(filename,"%s%cdebian%ccompat", directory, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);

	fp = fopen(filename,"w");
	if (!fp) return;
	sprintf(dh_version,"%s",DEBHELPER_VERSION);
	while ((i< strlen(dh_version)-1) && (dh_version[i]!='.')) {
		fprintf(fp, "%c", dh_version[i]);
		i++;
    }
	fprintf(fp,"\n");
	fclose(fp);
}

/* save a debian control file */
static void save_control()
{
	FILE * fp;
	int i, ctr, j, k;
	char str[77];

	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	char email_address[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char vcs_browser[BLOCK_SIZE];
	char vcs_repository[BLOCK_SIZE];
	char build_depends[BLOCK_SIZE];
	char depends[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char section[BLOCK_SIZE];

	directory[0]=0;
	filename[0]=0;
	email_address[0]=0;
	project_name[0]=0;
	homepage[0]=0;
	vcs_browser[0]=0;
	vcs_repository[0]=0;
	build_depends[0]=0;
	depends[0]=0;
	description_brief[0]=0;
	description[0]=0;
	section[0]=0;

	/* get the variables */
	get_setting("directory", directory);
	sprintf(filename,"%s%cdebian%ccontrol", directory, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);
	get_setting("email", email_address);
	get_setting("project", project_name);
	get_setting("homepage",homepage);
	get_setting("vcs browser", vcs_browser);
	get_setting("vcs repository", vcs_repository);
	get_setting("build depends", build_depends);
	get_setting("depends", depends);
	get_setting("description brief", description_brief);
	get_setting("description", description);
	get_setting("section", section);

	fp = fopen(filename,"w");
	if (!fp) return;

    fprintf(fp,"Source: %s\n", project_name);
	if (strlen(section)==0) {
		fprintf(fp,"Section: contrib/utils\n");
	}
	else {
		fprintf(fp,"Section: %s\n",section);
	}
    fprintf(fp,"Priority: extra\n");
    fprintf(fp,"Maintainer: %s\n",email_address);
    fprintf(fp,"Build-Depends: debhelper (>= %s)",DEBHELPER_VERSION);
	if (strlen(build_depends) > 0) {
		fprintf(fp,", %s",build_depends);
	}
	fprintf(fp,"\n");
	fprintf(fp,"Standards-Version: %s\n", DEBIAN_STANDARD);
	fprintf(fp,"Homepage: %s\n",homepage);
	if (strlen(vcs_browser) > 0) {
		fprintf(fp,"Vcs-Browser: %s\n", vcs_browser);
	}
	if (strlen(vcs_repository) > 0) {
		fprintf(fp,"Vcs-Git: %s\n",vcs_repository);
	}
	fprintf(fp,"\n");
    fprintf(fp,"Package: %s\n",project_name);
    fprintf(fp,"Architecture: any\n");
    fprintf(fp,"Depends: ${shlibs:Depends}, ${misc:Depends}");
	if (strlen(depends) > 0) {
		fprintf(fp,", %s",depends);
	}
	fprintf(fp,"\n");
    fprintf(fp,"Description: %s\n",description_brief);

	ctr = 0;
	str[ctr++] = ' ';
	for (i = 0; i < strlen(description); i++) {
		str[ctr++] = description[i];
		if (ctr == 76) {
			j = ctr-1;
			while (str[j] != ' ') {
				j--;
			}
			for (k = 0; k < j; k++) {
				fprintf(fp,"%c",str[k]);
			}
			fprintf(fp,"\n");
			for (k = j; k < ctr; k++) {
				str[k-j] = str[j];
			}
			ctr = ctr-j;
		}
		else {
			if (description[i]=='\n') {
				if (ctr > 0) {
					str[ctr] = 0;
					if (strlen(str) > 0) {
						fprintf(fp, "%s\n", str);
						fprintf(fp, " .\n");
					}
					ctr = 0;
					str[ctr++] = ' ';
				}
			}
		}
	}

	/* print the anything left over */
	if (ctr > 0) {
		str[ctr] = 0;
		if (strlen(str) > 1) {
			fprintf(fp, "%s\n", str);
		}
	}

    fclose(fp);
}


int save_debian()
{
	char debdir[BLOCK_SIZE];
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int retval;

	/* create the debian directory */
	get_setting("directory", directory);
	sprintf(debdir,"%s%cdebian", directory, DIRECTORY_SEPARATOR);
	sprintf(commandstr,"%s %s",COMMAND_MKDIR,debdir);
	retval = system(commandstr);

	save_compat();
	save_control();

	return retval;
}
