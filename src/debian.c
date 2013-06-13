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

char * sections_squeeze[] = {
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

char * sections_wheezy[] = {
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

/* returns the version of Debian for which the package will be created */
static float get_debian_version()
{
	char str[BLOCK_SIZE];

	str[0]=0;
	get_setting("debian version",str);
	if (strlen(str) > 0) {
		return atof(str);
	}
	return DEFAULT_DEBIAN_VERSION;
}

/* returns the Debian packaging standard based upon the OS version number */
static void get_debian_standard(char * standard)
{
	float debian_version = get_debian_version();
	
	if ((int)debian_version == 6) {
		sprintf(standard,"%s","3.9.2");
	}

	sprintf(standard,"%s","3.9.4");
}

/* returns the Debhelper version based upon the OS version */
static void get_debhelper_version(char * debhelper_version)
{
	float debian_version = get_debian_version();
	
	if ((int)debian_version == 6) {
		sprintf(debhelper_version, "%s", "8.0.0");
	}

	sprintf(debhelper_version, "%s", "9.0.0");
}

/* returns the possible section names for the given Debian version */
static int get_sections(char ***sections)
{
	float debian_version = get_debian_version();

	/* default */
	*sections = sections_wheezy;

	if ((int)debian_version == 6) {
		*sections = sections_squeeze;
		return 53;
	}

	return 53;
}

/* print all the section names */
void debian_list_sections()
{
	int i;
	char ** sections;
	int no_of_sections = get_sections(&sections);

	for (i = 0; i < no_of_sections; i++) {
		printf("%s\n",sections[i]);
	}
}

/* returns a non-zero value if the given section is valid */
int debian_valid_section(char * section)
{
	int i, j=-1, ctr=0;
	char subsection[BLOCK_SIZE];
	char ** sections;
	int no_of_sections = get_sections(&sections);

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

	get_debhelper_version(dh_version);

	get_setting("directory", directory);
	sprintf(filename,"%s%cdebian%ccompat", directory, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);

	fp = fopen(filename,"w");
	if (!fp) return;
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
	char standard[BLOCK_SIZE], dh_version[BLOCK_SIZE];

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

	get_debian_standard(standard);
	get_debhelper_version(dh_version);

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
    fprintf(fp,"Build-Depends: debhelper (>= %s)", dh_version);
	if (strlen(build_depends) > 0) {
		fprintf(fp,", %s",build_depends);
	}
	fprintf(fp,"\n");
	fprintf(fp,"Standards-Version: %s\n", standard);
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

/* GPL version of the copyright file */
static int save_copyright_gpl(char * filename, float version)
{
	FILE * fp;
	char email_address[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char vcs_browser[BLOCK_SIZE];
	time_t rawtime;
	struct tm * timeinfo;
	int year;

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;

	email_address[0]=0;
	project_name[0]=0;
    vcs_browser[0]=0;

	get_setting("email", email_address);
	get_setting("project", project_name);
	get_setting("vcs browser", vcs_browser);

	fp = fopen(filename,"w");
	if (!fp) return -1;

	fprintf(fp,"Format: http://www.debian.org/doc/packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",year,email_address);
	fprintf(fp,"License: GPL-%.1f+\n\n",version);

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",year,email_address);
	fprintf(fp,"License: GPL-%.1f+\n\n",version);

	fprintf(fp,"License: GPL-%.1f+\n",version);
	fprintf(fp," This program is free software: you can redistribute it and/or modify\n");
	fprintf(fp," it under the terms of the GNU General Public License as published by\n");
	fprintf(fp," the Free Software Foundation, either version %d of the License, or\n",(int)version);
	fprintf(fp," (at your option) any later version.\n .\n");

	fprintf(fp," This package is distributed in the hope that it will be useful,\n");
	fprintf(fp," but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(fp," MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	fprintf(fp," GNU General Public License for more details.\n .\n");

	fprintf(fp," You should have received a copy of the GNU General Public License\n");
	fprintf(fp," along with this program. If not, see <http://www.gnu.org/licenses/>.\n .\n");

	fprintf(fp," On Debian systems, the complete text of the GNU General\n");
	fprintf(fp," Public License version %d can be found in \"/usr/share/common-licenses/GPL-%d\".\n",
			(int)version,(int)version);

	fclose(fp);
	return 0;
}

/* BSD version of the copyright file */
static int save_copyright_bsd(char * filename)
{
	FILE * fp;
	char email_address[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char vcs_browser[BLOCK_SIZE];
	time_t rawtime;
	struct tm * timeinfo;
	int year;

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;

	email_address[0]=0;
	project_name[0]=0;
    vcs_browser[0]=0;

	get_setting("email", email_address);
	get_setting("project", project_name);
	get_setting("vcs browser", vcs_browser);

	fp = fopen(filename,"w");
	if (!fp) return -1;

	fprintf(fp,"Format: http://www.debian.org/doc/packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",year,email_address);
	fprintf(fp,"License: BSD-3-Clause\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",year,email_address);
	fprintf(fp,"License: BSD-3-Clause\n\n");

	fprintf(fp,"License: BSD-3-Clause\n");
	fprintf(fp," Redistribution and use in source and binary forms, with or without\n");
	fprintf(fp," modification, are permitted provided that the following conditions\n");
	fprintf(fp," are met:\n");
	fprintf(fp," 1. Redistributions of source code must retain the above copyright\n");
	fprintf(fp,"    notice, this list of conditions and the following disclaimer.\n");
	fprintf(fp," 2. Redistributions in binary form must reproduce the above copyright\n");
	fprintf(fp,"    notice, this list of conditions and the following disclaimer in the\n");
	fprintf(fp,"    documentation and/or other materials provided with the distribution.\n");
	fprintf(fp," 3. Neither the name of the University nor the names of its contributors\n");
	fprintf(fp,"    may be used to endorse or promote products derived from this software\n");
	fprintf(fp,"    without specific prior written permission.\n .\n");

	fprintf(fp," THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n");
	fprintf(fp," ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n");
	fprintf(fp," LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n");
	fprintf(fp," A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE HOLDERS OR\n");
	fprintf(fp," CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n");
	fprintf(fp," EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n");
	fprintf(fp," PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n");
	fprintf(fp," PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n");
	fprintf(fp," LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n");
	fprintf(fp," NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n");
	fprintf(fp," SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");

	fclose(fp);
	return 0;
}

/* save the copyright file */
static int save_copyright(char * directory)
{
	char license[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	int i;

	get_setting("license", license);

	/* the copyright path and filename */
	sprintf(filename, "%s%cdebian%ccopyright",
			directory, DIRECTORY_SEPARATOR,
			DIRECTORY_SEPARATOR);

	/* convert to lower case */
	for (i = 0; i < strlen(license); i++) {
		license[i] = tolower(license[i]);
	}

	/* GPL version 2 */
	if (strstr(license,"gpl2") != NULL) {
		return save_copyright_gpl(filename, 2.0f);
	}

	/* GPL version 3 */
	if (strstr(license,"gpl3") != NULL) {
		return save_copyright_gpl(filename, 3.0f);
	}

	/* BSD */
	if (strstr(license,"bsd") != NULL) {
		return save_copyright_bsd(filename);
	}
	return 0;
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
	save_copyright(directory);

	return retval;
}
