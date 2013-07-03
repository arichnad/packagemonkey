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

char * monthname[] = {
	"Jan","Feb","Mar","Apr","May","Jun",
	"Jul","Aug","Sep","Oct","Nov","Dec"
};

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

/* returns the version of Debian for which the package
   will be created */
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

/* returns the Debian packaging standard based upon the
   OS version number */
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

/* returns the possible section names for the given
   Debian version */
static int get_sections(char *** sections)
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
		if (((section[i] >= 'a') && (section[i] <= 'z')) ||
			(section[i] == '-') || (section[i] == '/')) {
			if (section[i]=='/') {
				ctr++;
				j = i;
			}
		}
		else {
			printf("%d %c %s\nSections should only contain lower " \
				   "case letters or hyphens, with no spaces\n",
				   (int)section[i], section[i], section);
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
		if (match_string(subsection, sections, \
						 no_of_sections) == 0) {
			printf("%s is not a permissable subsection name\n",
				   subsection);
			return 0;
		}

		/* the second subsection */
		for (i = j+1; i < strlen(section); i++) {
			subsection[i-(j+1)] = section[i];
		}
		subsection[i-(j+1)] = 0;
		if (match_string(subsection, sections, \
						 no_of_sections) == 0) {
			printf("%s is not a permissable subsection name\n",
				   subsection);
			return 0;
		}
	}
	else {
		if (match_string(section, sections, \
						 no_of_sections) == 0) {
			printf("%s is not a permissable subsection name\n",
				   section);
			return 0;
		}
	}

	return 1;
}

/* saves the compat file */
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
	sprintf(filename,"%s%c%s%ccompat", directory,
			DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);

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
	sprintf(filename,"%s%c%s%ccontrol", directory,
			DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);
	get_setting("email", email_address);
	get_setting("project name", project_name);
	get_setting("homepage",homepage);
	get_setting("vcs browser", vcs_browser);
	get_setting("vcs repository", vcs_repository);
	get_setting("description brief", description_brief);
	get_setting("description", description);
	get_setting("section", section);
	get_setting("depends deb", depends);
	get_setting("build deb", build_depends);

	get_debian_standard(standard);
	get_debhelper_version(dh_version);

	fp = fopen(filename,"w");
	if (!fp) return;

    fprintf(fp, "Source: %s\n", project_name);
    fprintf(fp, "%s", "Priority: extra\n");
    fprintf(fp, "Maintainer: %s\n", email_address);
    fprintf(fp, "Build-Depends: debhelper (>= %s)",
			dh_version);
	if (strlen(build_depends) > 0) {
		fprintf(fp, ", %s", build_depends);
	}
	fprintf(fp, "%s", "\n");
	fprintf(fp, "Standards-Version: %s\n", standard);
	fprintf(fp, "Homepage: %s\n", homepage);
	if (strlen(vcs_browser) > 0) {
		fprintf(fp, "Vcs-Browser: %s\n", vcs_browser);
	}
	if (strlen(vcs_repository) > 0) {
		fprintf(fp, "Vcs-Git: %s\n", vcs_repository);
	}

	fprintf(fp, "%s", "\n");
	if (is_library(project_name) == 0) {
		/* not a library */
		fprintf(fp, "Package: %s\n", project_name);
		if (strlen(section)==0) {
			fprintf(fp, "%s",
					"Section: contrib/utils\n");
		}
		else {
			fprintf(fp, "Section: %s\n", section);
		}
	}
	else {
		/* library */
		fprintf(fp, "Package: %s0\n", project_name);
		fprintf(fp, "%s", "Section: libs\n");
	}
    fprintf(fp, "%s", "Architecture: any\n");
    fprintf(fp, "%s", "Depends: ${shlibs:Depends}, " \
			"${misc:Depends}");
	if (strlen(depends) > 0) {
		fprintf(fp, ", %s", depends);
	}
	fprintf(fp, "%s", "\n");
    fprintf(fp, "Description: %s\n",description_brief);

	save_description(fp, description, 1);

	/* library headers package */
	if (is_library(project_name) != 0) {
		fprintf(fp, "%s", "\n");
		fprintf(fp, "Package: %s0-dev\n", project_name);
		fprintf(fp, "%s", "Section: libdevel\n");
		fprintf(fp, "%s", "Architecture: any\n");
		fprintf(fp, "Depends: %s0 " \
				"(= ${binary:Version}), " \
				"${shlibs:Depends}, ${misc:Depends}",
				project_name);
		if (strlen(depends) > 0) {
			fprintf(fp,", %s",depends);
		}
		fprintf(fp, "%s", "\n");
		fprintf(fp, "Description: %s\n",
				description_brief);

		save_description(fp, description, 1);
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: GPL-%.1f+\n\n",version);

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: GPL-%.1f+\n\n",version);

	fprintf(fp,"License: GPL-%.1f+\n",version);
	fprintf(fp," This program is free software: you " \
			"can redistribute it and/or modify\n");
	fprintf(fp," it under the terms of the GNU General " \
			"Public License as published by\n");
	fprintf(fp," the Free Software Foundation, either " \
			"version %d of the License, or\n",(int)version);
	fprintf(fp," (at your option) any later version.\n .\n");

	fprintf(fp," This package is distributed in the hope " \
			"that it will be useful,\n");
	fprintf(fp," but WITHOUT ANY WARRANTY; without even " \
			"the implied warranty of\n");
	fprintf(fp," MERCHANTABILITY or FITNESS FOR A " \
			"PARTICULAR PURPOSE.  See the\n");
	fprintf(fp," GNU General Public License for more " \
			"details.\n .\n");

	fprintf(fp," You should have received a copy of the " \
			"GNU General Public License\n");
	fprintf(fp," along with this program. If not, see " \
			"<http://www.gnu.org/licenses/>.\n .\n");

	fprintf(fp," On Debian systems, the complete text of " \
			"the GNU General\n");
	fprintf(fp," Public License version %d can be found " \
			"in \"/usr/share/common-licenses/GPL-%d\".\n",
			(int)version,(int)version);

	fclose(fp);
	return 0;
}

/* LGPL version 2 copyright file */
static int save_copyright_lgpl2(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: LGPL-2.1+\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: LGPL-2.1+\n\n");

	fprintf(fp,"%s","License: LGPL-2.1+\n");

	fprintf(fp,"%s"," This package is free software; you can redistribute it and/or\n");
	fprintf(fp,"%s"," modify it under the terms of the GNU Lesser General Public\n");
	fprintf(fp,"%s"," License as published by the Free Software Foundation; either\n");
	fprintf(fp,"%s"," version 2 of the License, or (at your option) any later version.\n .\n");

	fprintf(fp,"%s"," This package is distributed in the hope that it will be useful,\n");
	fprintf(fp,"%s"," but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(fp,"%s"," MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
	fprintf(fp,"%s"," Lesser General Public License for more details.\n .\n");

	fprintf(fp,"%s"," You should have received a copy of the GNU General Public License\n");
	fprintf(fp,"%s"," along with this program. If not, see <http://www.gnu.org/licenses/>.\n .\n");

	fprintf(fp,"%s"," On Debian systems, the complete text of the GNU Lesser General\n");
	fprintf(fp,"%s"," Public License can be found in \"/usr/share/common-licenses/LGPL-2\".\n");

	fclose(fp);
	return 0;
}

/* LGPL version 3 copyright file */
static int save_copyright_lgpl3(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: LGPL-3.0+\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: LGPL-3.0+\n\n");

	fprintf(fp,"%s","License: LGPL-3.0+\n");
	fprintf(fp,"%s"," This package is free software; you can redistribute it and/or\n");
	fprintf(fp,"%s"," modify it under the terms of the GNU Lesser General Public\n");
	fprintf(fp,"%s"," License as published by the Free Software Foundation; either\n");
	fprintf(fp,"%s"," version 3 of the License, or (at your option) any later version.\n .\n");

	fprintf(fp,"%s"," This package is distributed in the hope that it will be useful,\n");
	fprintf(fp,"%s"," but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(fp,"%s"," MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
	fprintf(fp,"%s"," Lesser General Public License for more details.\n .\n");

	fprintf(fp,"%s"," You should have received a copy of the GNU General Public License\n");
	fprintf(fp,"%s"," along with this program. If not, see <http://www.gnu.org/licenses/>.\n .\n");

	fprintf(fp,"%s"," On Debian systems, the complete text of the GNU Lesser General\n");
	fprintf(fp,"%s"," Public License can be found in \"/usr/share/common-licenses/LGPL-3\".\n");

	fclose(fp);
	return 0;
}

/* AGPL version 1 copyright file */
static int save_copyright_agpl1(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: AGPL-1.0\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: AGPL-1.0\n\n");

	fprintf(fp,"%s","License: AGPL-1.0\n");
	fprintf(fp,"%s"," This program is free software: you can redistribute it and/or modify\n");
	fprintf(fp,"%s"," it under the terms of the Affero General Public License as\n");
	fprintf(fp,"%s"," published by Affero, Inc.\n\n");

	fprintf(fp,"%s"," This program is distributed in the hope that it will be useful,\n");
	fprintf(fp,"%s"," but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(fp,"%s"," MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	fprintf(fp,"%s"," GNU Affero General Public License for more details.\n\n");

	fprintf(fp,"%s"," You should have received a copy of the Affero General Public License\n");
	fprintf(fp,"%s"," along with this program.  If not, see <http://www.affero.org/oagpl.html>.\n");

	fclose(fp);
	return 0;
}

/* AGPL version 3 copyright file */
static int save_copyright_agpl3(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: AGPL-3.0+\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"%s","License: AGPL-3.0+\n\n");

	fprintf(fp,"%s","License: AGPL-3.0+\n");
	fprintf(fp,"%s"," This program is free software: you can redistribute it and/or modify\n");
	fprintf(fp,"%s"," it under the terms of the GNU Affero General Public License as\n");
	fprintf(fp,"%s"," published by the Free Software Foundation, either version 3 of the\n");
	fprintf(fp,"%s"," License, or (at your option) any later version.\n\n");

	fprintf(fp,"%s"," This program is distributed in the hope that it will be useful,\n");
	fprintf(fp,"%s"," but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(fp,"%s"," MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	fprintf(fp,"%s"," GNU Affero General Public License for more details.\n\n");

	fprintf(fp,"%s"," You should have received a copy of the GNU Affero General Public License\n");
	fprintf(fp,"%s"," along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");

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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: BSD-3-Clause\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: BSD-3-Clause\n\n");

	fprintf(fp,"License: BSD-3-Clause\n");
	fprintf(fp," Redistribution and use in source and " \
			"binary forms, with or without\n");
	fprintf(fp," modification, are permitted provided " \
			"that the following conditions\n");
	fprintf(fp," are met:\n");
	fprintf(fp," 1. Redistributions of source code must " \
			"retain the above copyright\n");
	fprintf(fp,"    notice, this list of conditions and " \
			"the following disclaimer.\n");
	fprintf(fp," 2. Redistributions in binary form must " \
			"reproduce the above copyright\n");
	fprintf(fp,"    notice, this list of conditions and " \
			"the following disclaimer in the\n");
	fprintf(fp,"    documentation and/or other materials " \
			"provided with the distribution.\n");
	fprintf(fp," 3. Neither the name of the University " \
			"nor the names of its contributors\n");
	fprintf(fp,"    may be used to endorse or promote " \
			"products derived from this software\n");
	fprintf(fp,"    without specific prior written " \
			"permission.\n .\n");

	fprintf(fp," THIS SOFTWARE IS PROVIDED BY THE " \
			"COPYRIGHT HOLDERS AND CONTRIBUTORS\n");
	fprintf(fp," ``AS IS'' AND ANY EXPRESS OR IMPLIED " \
			"WARRANTIES, INCLUDING, BUT NOT\n");
	fprintf(fp," LIMITED TO, THE IMPLIED WARRANTIES OF " \
			"MERCHANTABILITY AND FITNESS FOR\n");
	fprintf(fp," A PARTICULAR PURPOSE ARE DISCLAIMED. " \
			"IN NO EVENT SHALL THE HOLDERS OR\n");
	fprintf(fp," CONTRIBUTORS BE LIABLE FOR ANY DIRECT, " \
			"INDIRECT, INCIDENTAL, SPECIAL,\n");
	fprintf(fp," EXEMPLARY, OR CONSEQUENTIAL DAMAGES " \
			"(INCLUDING, BUT NOT LIMITED TO,\n");
	fprintf(fp," PROCUREMENT OF SUBSTITUTE GOODS OR " \
			"SERVICES; LOSS OF USE, DATA, OR\n");
	fprintf(fp," PROFITS; OR BUSINESS INTERRUPTION) " \
			"HOWEVER CAUSED AND ON ANY THEORY OF\n");
	fprintf(fp," LIABILITY, WHETHER IN CONTRACT, STRICT " \
			"LIABILITY, OR TORT (INCLUDING\n");
	fprintf(fp," NEGLIGENCE OR OTHERWISE) ARISING IN ANY " \
			"WAY OUT OF THE USE OF THIS\n");
	fprintf(fp," SOFTWARE, EVEN IF ADVISED OF THE " \
			"POSSIBILITY OF SUCH DAMAGE.\n");

	fclose(fp);
	return 0;
}

/* MIT version of the copyright file */
static int save_copyright_mit(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: MIT\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: MIT\n\n");

	fprintf(fp,"License: MIT\n");
	fprintf(fp," Permission is hereby granted, free of " \
			"charge, to any person obtaining a\n");
	fprintf(fp," copy of this software and associated " \
			"documentation files (the \"Software\"),\n");
	fprintf(fp," to deal in the Software without " \
			"restriction, including without limitation\n");
	fprintf(fp," the rights to use, copy, modify, merge, " \
			"publish, distribute, sublicense,\n");
	fprintf(fp," and/or sell copies of the Software, and " \
			"to permit persons to whom the\n");
	fprintf(fp," Software is furnished to do so, subject " \
			"to the following conditions:\n .\n");

	fprintf(fp," The above copyright notice and this " \
			"permission notice shall be included\n");
	fprintf(fp," in all copies or substantial portions " \
			"of the Software.\n .\n");

	fprintf(fp," THE SOFTWARE IS PROVIDED \"AS IS\", " \
			"WITHOUT WARRANTY OF ANY KIND, EXPRESS\n");
	fprintf(fp," OR IMPLIED, INCLUDING BUT NOT LIMITED " \
			"TO THE WARRANTIES OF\n");
	fprintf(fp," MERCHANTABILITY, FITNESS FOR A PARTICULAR " \
			"PURPOSE AND NONINFRINGEMENT.\n");
	fprintf(fp," IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT " \
			"HOLDERS BE LIABLE FOR ANY\n");
	fprintf(fp," CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER " \
			"IN AN ACTION OF CONTRACT, \n");
	fprintf(fp," TORT OR OTHERWISE, ARISING FROM, OUT OF OR " \
			"IN CONNECTION WITH THE \n");
	fprintf(fp," SOFTWARE OR THE USE OR OTHER DEALINGS IN " \
			"THE SOFTWARE.\n");

	fclose(fp);
	return 0;
}

/* Apache version of the copyright file */
static int save_copyright_apache(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: Apache-2.0\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: Apache-2.0\n\n");

	fprintf(fp,"License: Apache-2.0\n");
	fprintf(fp," Licensed under the Apache License, " \
			"Version 2.0 (the \"License\");\n");
	fprintf(fp," you may not use this file except in " \
			"compliance with the License.\n");
	fprintf(fp," You may obtain a copy of the License at\n .\n");

	fprintf(fp," http://www.apache.org/licenses/LICENSE-2.0\n .\n");

	fprintf(fp," Unless required by applicable law or " \
			"agreed to in writing, software\n");
	fprintf(fp," distributed under the License is " \
			"distributed on an \"AS IS\" BASIS,\n");
	fprintf(fp," WITHOUT WARRANTIES OR CONDITIONS OF " \
			"ANY KIND, either express or implied.\n");
	fprintf(fp," See the License for the specific " \
			"language governing permissions and\n");
	fprintf(fp," limitations under the License.\n .\n");

	fprintf(fp," On Debian systems, the complete text " \
			"of the Apache version 2.0 license\n");
	fprintf(fp," can be found in \"/usr/share/common-" \
			"licenses/Apache-2.0\".\n");

	fclose(fp);
	return 0;
}

/* Mozilla version of the copyright file */
static int save_copyright_mozilla(char * filename)
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

	fprintf(fp,"Format: http://www.debian.org/doc/" \
			"packaging-manuals/copyright-format/1.0/\n");
	fprintf(fp,"Upstream-Name: %s\n",project_name);
	fprintf(fp,"Source: %s\n\n",vcs_browser);

	fprintf(fp,"Files: *\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: MPL-2.0\n\n");

	fprintf(fp,"Files: debian/*\n");
	fprintf(fp,"Copyright: Copyright %d %s\n",
			year,email_address);
	fprintf(fp,"License: MPL-2.0\n\n");

	fprintf(fp,"License: MPL-2.0\n");
	fprintf(fp," This Source Code Form is subject to the terms\n");
	fprintf(fp," of the Mozilla Public License, v. 2.0. If a copy\n");
	fprintf(fp," of the MPL was not distributed with this file,\n");
	fprintf(fp," You can obtain one at http://mozilla.org/MPL/2.0/.\n");
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
	sprintf(filename, "%s%c%s%ccopyright",
			directory, DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);

	/* convert to lower case */
	for (i = 0; i < strlen(license); i++) {
		license[i] = tolower(license[i]);
	}

	/* GPL version 2 */
	if (strstr(license,"gpl2") != NULL) {
		return save_copyright_gpl(filename, 2.0f);
	}

	/* LGPL version 2 */
	if (strstr(license,"lgpl2") != NULL) {
		return save_copyright_lgpl2(filename);
	}

	/* GPL version 3 */
	if (strstr(license,"gpl3") != NULL) {
		return save_copyright_gpl(filename, 3.0f);
	}

	/* LGPL version 3 */
	if (strstr(license,"lgpl3") != NULL) {
		return save_copyright_lgpl3(filename);
	}

	/* AGPL version 1 */
	if (strstr(license,"agpl") != NULL) {
		return save_copyright_agpl1(filename);
	}

	/* AGPL version 3 */
	if (strstr(license,"agpl3") != NULL) {
		return save_copyright_agpl3(filename);
	}

	/* BSD */
	if (strstr(license,"bsd") != NULL) {
		return save_copyright_bsd(filename);
	}

	/* MIT */
	if (strstr(license,"mit") != NULL) {
		return save_copyright_mit(filename);
	}

	/* Apache */
	if (strstr(license,"apache") != NULL) {
		return save_copyright_apache(filename);
	}

	/* Mozilla */
	if ((strstr(license,"mozilla") != NULL) ||
		(strstr(license,"mpl") != NULL)) {
		return save_copyright_mozilla(filename);
	}
	return 0;
}

/* saves the Debian rules file */
static int save_rules(char * directory,
					  int no_of_binaries, char ** binaries)
{
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char project_type[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char commandline[BLOCK_SIZE];
	FILE * fp;

	/* rules file path and filename */
	sprintf(filename, "%s%c%s%crules",
			directory, DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);

	get_setting("project name",project_name);
	get_setting("project type",project_type);
	get_setting("commandline",commandline);

	fp = fopen(filename,"w");
	if (!fp) return 0;

	fprintf(fp,"%s","#!/usr/bin/make -f\n\n");

	fprintf(fp,"APP=%s\n",project_name);
	if (strlen(project_type) != 0) {
		fprintf(fp,"%s","application = $(CURDIR)/$(APP)\n");
	}
	fprintf(fp, "DESTDIR=$(CURDIR)/%s/$(APP)\n",
			DEB_SUBDIR);

	fprintf(fp,"%s","build: build-stamp\n");
	fprintf(fp,"%s","\tmake\n");
	fprintf(fp,"%s","build-arch: build-stamp\n");
	fprintf(fp,"%s","build-indep: build-stamp\n");
	fprintf(fp,"%s","build-stamp:\n");
	fprintf(fp,"%s","		dh_testdir\n");
	fprintf(fp,"%s","		touch build-stamp\n\n");

	fprintf(fp,"%s","clean:\n");
	fprintf(fp,"%s","		dh_testdir\n");
	fprintf(fp,"%s","		dh_testroot\n");
	fprintf(fp,"%s","		rm -f build-stamp\n");
	fprintf(fp,"%s","		dh_clean\n\n");

	if (strlen(project_type) != 0) {
		fprintf(fp,"%s","install: build clean $(application)\n");
	}
	else {
		fprintf(fp,"%s","install: build clean\n");
	}
	fprintf(fp,"%s","		 dh_testdir\n");
	fprintf(fp,"%s","		 dh_testroot\n");
	fprintf(fp,"%s","		 dh_prep\n");
	fprintf(fp,"%s","		 dh_installdirs\n");
	fprintf(fp,"		 ${MAKE} install -B DESTDIR=$(CURDIR)/%s/$(APP)\n",DEB_SUBDIR);

	fprintf(fp,"%s","binary-indep: build install\n");
	fprintf(fp,"%s","			  dh_shlibdeps\n"); /* TODO check */
	fprintf(fp,"%s","			  dh_testdir\n");
	fprintf(fp,"%s","			  dh_testroot\n");
	fprintf(fp,"%s","			  dh_installchangelogs\n");
	fprintf(fp,"%s","			  dh_installdocs\n");
	fprintf(fp,"%s","			  dh_installexamples\n");
	fprintf(fp,"%s","			  dh_installman\n");
	fprintf(fp,"%s","			  dh_link\n");
	fprintf(fp,"%s","			  dh_compress\n");
	fprintf(fp,"%s","			  dh_fixperms\n");
	fprintf(fp,"%s","			  dh_installdeb\n");
	fprintf(fp,"%s","			  dh_gencontrol\n");
	fprintf(fp,"%s","			  dh_md5sums\n");
	fprintf(fp,"%s","			  dh_builddeb\n\n");

	fprintf(fp,"%s","binary-arch: build install\n\n");

	fprintf(fp,"%s","binary: binary-indep binary-arch\n");
	fprintf(fp,"%s",".PHONY: build clean binary-indep " \
			"binary-arch binary install\n");

	fclose(fp);

	sprintf(commandstr,"chmod +x %s", filename);
	return system(commandstr);
}

/* generate a manpage */
static void save_manpages(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char manpage[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char email_address[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	time_t rawtime;
	struct tm * timeinfo;
	int year,month,day;

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;
	month = timeinfo->tm_mon;
	day = timeinfo->tm_mday;

	sprintf(filename,"%s%c%s%cmanpages",
			directory, DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);

	get_setting("project name",project_name);

	/* path and filename of the manpage */
	sprintf(manpage,"%s%cman%c%s.1.gz",
			directory,
			DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR,
			project_name);

	/* save debian/manpages */
	fp = fopen(filename,"w");
	if (!fp) return;
	fprintf(fp,"man/%s.1.gz\n",project_name);
	fclose(fp);

	if (file_exists(manpage)!=0) return;

	/* create an empty manpage */
	sprintf(filename,"%s%cman%c%s.1",
			directory,
			DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR,
			project_name);
	fp = fopen(filename,"w");
	if (!fp) return;
	fprintf(fp,".TH %s 1 \"%02d %s %d\" \"\" " \
			"\"User's Manual\"\n\n",
			project_name, day, monthname[month], year);

	fprintf(fp,".SH \"NAME\"\n");
	get_setting("description brief",description_brief);
	fprintf(fp,"%s \\- %s\n\n",project_name,description_brief);

	fprintf(fp,".SH \"DESCRIPTION\"\n");
	get_setting("description",description);
	fprintf(fp,"%s\n\n",description);

	fprintf(fp,".SH \"INSTALLATION\"\n\n");
	fprintf(fp,"Installation instructions go here ...\n\n");

	fprintf(fp,".SH \"SEE ALSO\"\n\n");
	fprintf(fp,"Any related manpages ...\n\n");

	fprintf(fp,".SH \"BUGS\"\n");
	fprintf(fp,"Report bugs at ...\n\n");

	fprintf(fp,".SH \"AUTHOR\"\n");
	get_setting("email",email_address);
	get_setting("homepage",homepage);
	fprintf(fp,"%s\n",email_address);
	fprintf(fp,".br\n.I %s\n",homepage);
	fclose(fp);

	/* gzip the file */
	sprintf(commandstr,"gzip %s",filename);
	day = system(commandstr);
}

static void save_changelog(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char project_version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char email_address[BLOCK_SIZE];
	time_t rawtime;
	struct tm * timeinfo;
	int year,month,day,weekday,hour,min;
	char * dayname[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};

	sprintf(filename,"%s%c%s%cchangelog",directory,
			DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);

	/* does it already exist? */
	if (file_exists(filename) != 0) return;

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;
	month = timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	weekday = timeinfo->tm_wday;
	hour = timeinfo->tm_hour;
	min = timeinfo->tm_min;

	get_setting("project name",project_name);
	get_setting("version",project_version);
	get_setting("release",release);
	get_setting("email",email_address);

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"%s (%s-%s) stable; urgency=medium\n\n",
			project_name, project_version, release);
	fprintf(fp,"  * Initial release\n\n");
	fprintf(fp," -- %s  %s, %02d %s %d %02d:%02d:00 +0100\n",
			email_address, dayname[weekday],
			day, monthname[month], year,
			hour, min);

	fclose(fp);
}


/* file containing list of binaries to be included
   in the source */
static void save_include_binary(char * directory,
								int no_of_binaries, char ** binaries)
{
	FILE * fp;
	int i;
	char project_name[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	sprintf(filename,"%s%cinclude-binaries",directory,
			DIRECTORY_SEPARATOR);

	if (file_exists(filename) != 0) return;

	get_setting("project name",project_name);

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"man/%s.1.gz\n",project_name);

	for (i = 0; i < no_of_binaries; i++) {
		if (binaries[i][0] != DIRECTORY_SEPARATOR) {
			fprintf(fp,"%s\n",binaries[i]);
		}
	}

	fclose(fp);
}

/* save the format file */
static void save_format(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];

	sprintf(filename,"%s%cformat",directory,
			DIRECTORY_SEPARATOR);

	if (file_exists(filename) != 0) return;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"3.0 (quilt)");

	fclose(fp);
}

/* save the debian/docs file */
static void save_docs(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];

	sprintf(filename,"%s%c%s%cdocs", directory,
			DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);

	if (file_exists(filename) != 0) return;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"README.md\n");

	fclose(fp);
}

/* readme file */
static void save_readme(char * directory)
{
	FILE * fp;
	int i;
	char project_name[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	sprintf(filename,"%s%cREADME.md",
			directory,DIRECTORY_SEPARATOR);

	if (file_exists(filename) != 0) return;

	get_setting("project name",project_name);

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"%s\n",project_name);
	for (i = 0; i < strlen(project_name); i++) {
		fprintf(fp,"%c",'=');
	}
	fprintf(fp,"\n\n");

	fclose(fp);
}

/* saves a script which can be used to build the package */
static int save_debian_build_script(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char project_version[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char release[BLOCK_SIZE];

	sprintf(filename,"%s%cdebian.sh",
			directory, DIRECTORY_SEPARATOR);

	get_setting("project name",project_name);
	get_setting("version",project_version);
	get_setting("release",release);

	if (file_exists(filename) != 0) {
		/* if the script already exists then
		   change the application name
		   and version number if needed */
		replace_build_script_version(filename,
									 project_name,
									 project_version);
		return 0;
	}

	fp = fopen(filename,"w");
	if (!fp) return 0;

	fprintf(fp,"#!/bin/bash\n\n");

	fprintf(fp,"APP=%s\n",project_name);
	fprintf(fp,"PREV_VERSION=%s\n",project_version);
	fprintf(fp,"VERSION=%s\n",project_version);
	fprintf(fp,"RELEASE=%s\n",release);
	fprintf(fp, "%s", "ARCH_TYPE=`uname -m`\n");
	fprintf(fp, "%s", "DIR=${APP}-${VERSION}\n\n");

	fprintf(fp, "%s", "if [ $ARCH_TYPE == \"x86_64\" ]; then\n");
	fprintf(fp, "%s", "    ARCH_TYPE=\"amd64\"\n");
	fprintf(fp, "%s", "fi\n");
	fprintf(fp, "%s", "if [ $ARCH_TYPE == \"i686\" ]; then\n");
	fprintf(fp, "%s", "    ARCH_TYPE=\"i386\"\n");
	fprintf(fp, "%s", "fi\n\n");

	/* alter the version numbers */
	script_version_numbers(fp,"debian");

	fprintf(fp, "%s", "make clean\n");
	fprintf(fp, "%s", "make\n\n");

	fprintf(fp, "%s", "# change the parent directory name " \
			"to debian format\n");
	fprintf(fp, "%s", "mv ../${APP} ../${DIR}\n\n");

	fprintf(fp, "%s", "# Create a source archive\n");
	fprintf(fp, "%s", "make source\n\n");

	fprintf(fp, "%s", "# Build the package\n");
	fprintf(fp, "%s", "dpkg-buildpackage -F\n\n");

	fprintf(fp, "%s", "# sign files\n");
	fprintf(fp, "%s", "gpg -ba ../${APP}_${VERSION}-1_$" \
			"{ARCH_TYPE}.deb\n");
	fprintf(fp, "%s", "gpg -ba ../${APP}_${VERSION}.orig.tar.gz\n\n");

	fprintf(fp, "%s", "# restore the parent directory name\n");
	fprintf(fp, "%s", "mv ../${DIR} ../${APP}\n");

	fclose(fp);

	sprintf(commandstr,"chmod +x %s", filename);
	return system(commandstr);
}

/* creates dirs files for libraries */
static int save_library_dirs(char * directory)
{
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	FILE * fp;
	int i;

	get_setting("project name", project_name);

	if (is_library(project_name) == 0) return 0;

	get_setting("version", version);
	
	for (i = 0; i < 2; i++) {
		if (i == 0) {
			sprintf(filename,"%s%c%s%c%s0.dirs",
					directory, DIRECTORY_SEPARATOR,
					DEB_SUBDIR, DIRECTORY_SEPARATOR,
					project_name);
		}
		else {
			sprintf(filename,"%s%c%s%c%s-dev.dirs",
					directory, DIRECTORY_SEPARATOR,
					DEB_SUBDIR, DIRECTORY_SEPARATOR,
					project_name);
		}
		fp = fopen(filename, "w");
		if (!fp) return -1;

		fprintf(fp,"%s","usr/lib\n");
		if (i == 1) {
			fprintf(fp,"usr/include/%s\n",project_name);
		}

		fclose(fp);
	}
	return 1;
}

/* creates links files for libraries */
static int save_library_links(char * directory)
{
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	FILE * fp;
	int i;

	get_setting("project name", project_name);

	if (is_library(project_name) == 0) return 0;

	get_setting("version", version);
	
	for (i = 0; i < 2; i++) {
		if (i == 0) {
			sprintf(filename,"%s%c%s%c%s0.links",
					directory, DIRECTORY_SEPARATOR,
					DEB_SUBDIR, DIRECTORY_SEPARATOR,
					project_name);
		}
		else {
			sprintf(filename,"%s%c%s%c%s0-dev.links",
					directory, DIRECTORY_SEPARATOR,
					DEB_SUBDIR, DIRECTORY_SEPARATOR,
					project_name);
		}
		fp = fopen(filename, "w");
		if (!fp) return -1;

		fprintf(fp,"usr/lib/%s-%s.so.0.0.1 usr/lib/%s.so.0\n",
				project_name, version, project_name);

		fclose(fp);
	}
	return 1;
}

/* creates install files for libraries */
static int save_library_install(char * directory)
{
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	FILE * fp;
	int i;

	get_setting("project name", project_name);

	if (is_library(project_name) == 0) return 0;

	get_setting("version", version);
	
	for (i = 0; i < 2; i++) {
		if (i == 0) {
			sprintf(filename,"%s%c%s%c%s0.install",
					directory, DIRECTORY_SEPARATOR,
					DEB_SUBDIR, DIRECTORY_SEPARATOR,
					project_name);
		}
		else {
			sprintf(filename,"%s%c%s%c%s-dev.install",
					directory, DIRECTORY_SEPARATOR,
					DEB_SUBDIR, DIRECTORY_SEPARATOR,
					project_name);
		}
		fp = fopen(filename, "w");
		if (!fp) return -1;

		fprintf(fp,"%s","usr/lib/lib*.so\n");
		fprintf(fp,"%s","usr/lib/lib*.so.*\n");
		if (i == 1) {
			fprintf(fp,"usr/include/%s/*\n",project_name);
			fprintf(fp,"%s","usr/lib*.a\n");
			fprintf(fp,"%s","usr/lib/*.la\n");
			fprintf(fp,"%s","usr/lib/pkgconfig/*\n");
			fprintf(fp,"%s","usr/share/pkgconfig/*\n");
		}

		fclose(fp);
	}
	return 1;
}

int save_debian(int no_of_binaries, char ** binaries)
{
	char debdir[BLOCK_SIZE];
	char debsourcedir[BLOCK_SIZE];
	char sourcedir[BLOCK_SIZE];
	char mandir[BLOCK_SIZE];
	char desktopdir[BLOCK_SIZE];
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char commandline[BLOCK_SIZE];
	int retval=0,i;

	get_setting("commandline",commandline);

	/* create the debian directory */
	get_setting("directory", directory);
	sprintf(debdir,"%s%c%s",
			directory, DIRECTORY_SEPARATOR,DEB_SUBDIR);
	if (directory_exists(debdir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,debdir);
		retval = system(commandstr);
	}
	/* create debian/source */
	sprintf(debsourcedir,"%s%c%s%csource",
			directory, DIRECTORY_SEPARATOR,
			DEB_SUBDIR, DIRECTORY_SEPARATOR);
	if (directory_exists(debsourcedir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,debsourcedir);
		retval = system(commandstr);
	}
	/* create src */
	sprintf(sourcedir,"%s%csrc",
			directory, DIRECTORY_SEPARATOR);
	if (directory_exists(sourcedir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,sourcedir);
		retval = system(commandstr);
	}
	/* create man */
	sprintf(mandir,"%s%cman",
			directory, DIRECTORY_SEPARATOR);
	if (directory_exists(mandir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,mandir);
		retval = system(commandstr);
	}
	if (strlen(commandline)==0) {
	/* create desktop */
		sprintf(desktopdir,"%s%cdesktop",
				directory, DIRECTORY_SEPARATOR);
		if (directory_exists(desktopdir)==0) {
			sprintf(commandstr,"%s %s",COMMAND_MKDIR,desktopdir);
			retval = system(commandstr);
		}
	}

	/* move any code into the src directory */
	int no_of_extensions = 18;
	char * extensions[] =  {
		"c","cpp","h","py","vala","java","rbbas","rbuistate","rbmnu","rbfrm",
		"rbtbar","rbvcp","rbres","ico","png","jpg", "jpeg", "gif"
	};
	for (i = 0; i < no_of_extensions; i++) {
		if (files_exist(directory,extensions[i]) != 0) {
			sprintf(commandstr,
					"cd %s; %s *.%s src",
					directory, COMMAND_MOVE, extensions[i]);
			retval = system(commandstr);
		}
	}

	/* remove any object files */
	sprintf(commandstr,
			"cd %s; %s *.o *.pyc 2>&1",
			directory, COMMAND_DELETE);
	retval = system(commandstr);

	save_compat();
	save_control();
	save_copyright(directory);
	save_rules(directory,
			   no_of_binaries, binaries);
	save_manpages(directory);
	save_changelog(directory);
	save_include_binary(debsourcedir,
						no_of_binaries, binaries);
	save_format(debsourcedir);
	save_readme(directory);
	save_docs(directory);
    save_debian_build_script(directory);
	save_library_links(directory);
	save_library_dirs(directory);
	save_library_install(directory);

	return retval;
}
