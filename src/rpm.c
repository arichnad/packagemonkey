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

#include "rpm.h"

static int save_spec(char * project_directory,
					 char * directory,
					 int no_of_binaries, char ** binaries)
{
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char email_address[BLOCK_SIZE];
	char requires[BLOCK_SIZE];
	char build_requires[BLOCK_SIZE];
	char license[BLOCK_SIZE];
	char svg_filename[BLOCK_SIZE];
	char source_package[BLOCK_SIZE];
	char categories[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char commandline[BLOCK_SIZE];
	char * directories[MAX_FILES];
	FILE * fp;
	int ctr,i,j=0,k,no_of_directories;
	char str[BLOCK_SIZE];
	const char * dayname[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	const char * monthname[] = {
		"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
	};
	time_t rawtime;
	struct tm * timeinfo;
	int year,month,day,weekday;

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;
	month = timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	weekday = timeinfo->tm_wday;

	get_setting("project name", project_name);
	if (strlen(project_name) == 0) return -1;

	sprintf(filename,"%s%c%s.spec",
			directory, DIRECTORY_SEPARATOR,
			project_name);

	fp = fopen(filename,"w");
	if (!fp) return -1;

	get_setting("version", version);
	get_setting("description brief", description_brief);
	get_setting("description", description);
	get_setting("license", license);
	get_setting("homepage", homepage);
	get_setting("email", email_address);
	get_setting("depends rpm", requires);
	get_setting("build rpm", build_requires);
	get_setting("source package", source_package);
	get_setting("categories", categories);
	get_setting("release", release);
	get_setting("commandline", commandline);

	fprintf(fp,"Name: %s\n",project_name);
	fprintf(fp,"Version: %s\n",version);
	fprintf(fp,"Release: %s%%{?dist}\n",release);
	fprintf(fp,"Summary: %s\n",description_brief);
	fprintf(fp,"License: %s\n",license);
	fprintf(fp,"URL: %s\n",homepage);
	fprintf(fp,"Packager: %s\n", email_address);   
	if (strlen(source_package) > 0) {
		fprintf(fp,"Source0: %s\n", source_package);
	}
	else {
		fprintf(fp,"%s", "Source0: http://yourdomainname.com/src/%{name}_%{version}.orig.tar.gz\n");
	}
	if (strlen(categories) > 0) {
		fprintf(fp,"Group: %s\n",categories);
	}
	else {
		fprintf(fp,"%s","Group: Application/Utility\n");
	}

	if (strlen(requires) > 0) {
		fprintf(fp,"Requires: %s\n",requires);
	}
	if (strlen(build_requires) > 0) {
		fprintf(fp,"BuildRequires: %s\n",build_requires);
	}
	fprintf(fp,"%s","\n%description\n");
	
	ctr = 0;
	for (i = 0; i < strlen(description); i++) {
		str[ctr++] = description[i];
		if (ctr == 76) {
			j = ctr-1;
			if (str[j] != ' ') {
				while (str[j] != ' ') {
					j--;
				}
			}
			while (str[j] == ' ') {
				j--;
			}
			j++;
			for (k = 0; k < j; k++) {
				fprintf(fp,"%c", str[k]);
			}
			fprintf(fp,"\n");
			i -= ctr-j-1;
			ctr = 0;		
		}
		else {
			if (description[i]=='\n') {
				if (ctr > 0) {
					str[ctr] = 0;
					if (strlen(str) > 0) {
						fprintf(fp, "%s\n", str);
						fprintf(fp, "\n");
					}
					ctr = 0;
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

	sprintf(svg_filename,"%s%cdesktop%cicon.svg",
			project_directory, DIRECTORY_SEPARATOR,
			DIRECTORY_SEPARATOR);

	fprintf(fp,"%s","\n%prep\n");
	fprintf(fp,"%s","%setup -q\n\n");

	fprintf(fp,"%s","%build\n");
	fprintf(fp,"%s","%configure\n");
	fprintf(fp,"%s","make %{?_smp_mflags}\n\n");

	fprintf(fp,"%s","%install\n");
	fprintf(fp,"%s","rm -rf %{buildroot}\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}/etc\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}/etc/%{name}\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}/usr\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}/usr/bin\n");

	/* create directories for binaries */
	if (no_of_binaries > 0) {
		no_of_directories =
			get_directories(binaries, no_of_binaries,
							directories);
		for (i = 0; i < no_of_directories; i++) {
			if (get_subdirectory_string(directories[i]) != 0) {
				fprintf(fp,"mkdir -p \"%%{buildroot}/%s\"\n",
						get_subdirectory_string(directories[i]));
			}
			free(directories[i]);
		}
	}

	if (is_library(project_name) != 0) {
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/lib\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/lib/%{name}\n");
	}
	fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/man\n");
	fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/man/man1\n");

	if (strlen(commandline) == 0) {
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/%{name}\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/applications\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/pixmaps\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/icons\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/icons/hicolor\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/icons/hicolor/scalable\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/icons/hicolor/scalable/apps\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/icons/hicolor/24x24\n");
		fprintf(fp,"%s","mkdir -p %{buildroot}/usr/share/icons/hicolor/24x24/apps\n\n");
	}

	fprintf(fp,"%s","# Make install but to the RPM BUILDROOT directory\n");	
	fprintf(fp,"%s","make install -B DESTDIR=%{buildroot}\n\n");

	fprintf(fp,"%s","%files\n");
	fprintf(fp,"%s","%doc README.md LICENSE\n");
	fprintf(fp,"%s","%defattr(-,root,root,-)\n");
	fprintf(fp,"%s","%{_bindir}/*\n");
	fprintf(fp,"%s","%{_mandir}/man1/*\n");
	if (strlen(commandline) == 0) {
		fprintf(fp,"%s","%attr(644,root,root) /usr/share/applications/%{name}.desktop\n");
		fprintf(fp,"%s","%attr(644,root,root) /usr/share/icons/hicolor/24x24/apps/%{name}.png\n");
		if (file_exists(svg_filename) != 0) {
			fprintf(fp,"%s","%attr(644,root,root) /usr/share/icons/hicolor/scalable/apps/%{name}.svg\n");
			fprintf(fp,"%s","%attr(644,root,root) /usr/share/pixmaps/%{name}.svg\n");
		}
	}
	if (is_library(project_name) == 0) {
		/* install some binaries */
		for (i = 0; i < no_of_binaries; i++) {
			if (contains_char(get_subdirectory_string(binaries[i]),' ')==0) {
				fprintf(fp,"%%attr(755,root,root) /%s\n",
						get_subdirectory_string(binaries[i]));
			}
			else {
				fprintf(fp,"%%attr(755,root,root) \"/%s\"\n",
						get_subdirectory_string(binaries[i]));
			}
		}
	}
	else {
		/* install libraries */
		for (i = 0; i < no_of_binaries; i++) {
			fprintf(fp,"%%attr(755,root,root) /usr/lib/%%{name}/%s\n",
					get_subdirectory_string(binaries[i]));
		}
	}

	fprintf(fp,"%s","\n");

	if (is_library(project_name) != 0) {
		fprintf(fp,"%s","%post\n");
		fprintf(fp,"%s","umask 007\n");
		fprintf(fp,"%s","ldconfig > /dev/null 2>&1\n");

		for (i = 0; i < no_of_binaries; i++) {
			for (j = strlen(binaries[i])-1; j >= 0; j--) {
				if (binaries[i][j] == DIRECTORY_SEPARATOR) {
					j++;
					break;
				}
			}
			if (file_is_library(&binaries[i][j]) != 0) {
				fprintf(fp,"ln -sf /usr/lib/%s.0.0.1 /usr/lib/%%{name}/%s\n",
						&binaries[i][j], &binaries[i][j]);
			}
		}
		fprintf(fp,"%s","\n");

		fprintf(fp,"%s","%postun\n");
		fprintf(fp,"%s","umask 007\n");
		fprintf(fp,"%s","ldconfig > /dev/null 2>&1\n");
		for (i = 0; i < no_of_binaries; i++) {
			for (j = strlen(binaries[i])-1; j >= 0; j--) {
				if (binaries[i][j] == DIRECTORY_SEPARATOR) {
					j++;
					break;
				}
            }
			if (file_is_library(&binaries[i][j]) != 0) {
				fprintf(fp,"rm /usr/lib/%s\n",&binaries[i][j]);
			}
        }
		fprintf(fp,"%s","\n");
	}

	fprintf(fp,"%s","%changelog\n");
	fprintf(fp,"* %s %s %d %d  %s\n",
			dayname[weekday], monthname[month], day, year, email_address);
	fprintf(fp,"%s","- Spec file created\n");

	fclose(fp);
	return 0;
}

static int save_script(char * directory, char * subdir)
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

	sprintf(filename,"%s%crpm.sh",
			directory, DIRECTORY_SEPARATOR);

	if (file_exists(filename) != 0) {
		/* if the script already exists then
		   change the version number and app name */
		replace_build_script_version(filename,
									 project_name,
									 version);
		return -1;
	}

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
	fprintf(fp, "%s", "SOURCE=~/rpmbuild/SOURCES/${APP}-${VERSION}.orig.tar.gz\n\n");

	fprintf(fp, "%s", "#update version numbers automatically - so you don't have to\n");
	fprintf(fp, "%s", "sed -i 's/VERSION='${PREV_VERSION}'/VERSION='${VERSION}'/g' Makefile debian.sh\n");
	fprintf(fp, "sed -i 's/Version: '${PREV_VERSION}'/Version: '${VERSION}'/g' %s/${APP}.spec\n",subdir);
	fprintf(fp, "sed -i 's/Release: '${RELEASE}'/Release: '${RELEASE}'/g' %s/${APP}.spec\n\n", subdir);

	fprintf(fp, "%s", "sudo yum groupinstall \"Development Tools\"\n");
	fprintf(fp, "%s", "sudo yum install rpmdevtools\n\n");

	fprintf(fp, "%s", "# setup the rpmbuild directory tree\n");
	fprintf(fp, "%s", "rpmdev-setuptree\n\n");

	fprintf(fp, "%s", "# create the source code in the SOURCES directory\n");
	fprintf(fp, "%s", "make clean\n");
	fprintf(fp, "%s -p ~/rpmbuild/SOURCES\n", COMMAND_MKDIR);
	fprintf(fp, "%s", "rm -f ${SOURCE}\n");
	fprintf(fp, "%s", "# having the root directory called name-version seems essential\n");
	fprintf(fp, "%s", "mv ../${APP} ../${APP}-${VERSION}\n");
	fprintf(fp, "%s", "tar -cvzf ${SOURCE} ../${APP}-${VERSION} --exclude-vcs\n");
	fprintf(fp, "%s", "# rename the root directory without the version number\n");
	fprintf(fp, "%s", "mv ../${APP}-${VERSION} ../${APP}\n\n");

	fprintf(fp, "%s", "# copy the spec file into the SPECS directory\n");
	fprintf(fp, "cp -f %s/${APP}.spec ~/rpmbuild/SPECS\n\n",subdir);

	fprintf(fp, "%s", "# build\n");
	fprintf(fp, "%s", "cd ~/rpmbuild/SPECS\n");
	fprintf(fp, "%s", "rpmbuild -ba ${APP}.spec\n");
	fprintf(fp, "%s", "cd ${CURRDIR}\n\n");

	fprintf(fp, "# Copy the results into the %s directory\n", subdir);
	fprintf(fp, "mkdir -p %s/${ARCH_TYPE}\n", subdir);
	fprintf(fp, "cp -r ~/rpmbuild/RPMS/${ARCH_TYPE}/${APP}* %s/${ARCH_TYPE}\n", subdir);
	fprintf(fp, "cp -r ~/rpmbuild/SRPMS/${APP}* %s\n", subdir);

	fclose(fp);

	sprintf(commandstr,"chmod +x %s", filename);
	return system(commandstr);
}

int save_rpm(int no_of_binaries, char ** binaries)
{
	char rpmdir[BLOCK_SIZE];
	char subdir[BLOCK_SIZE];
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int retval=0;

	/* create the debian directory */
	get_setting("directory", directory);
	sprintf(subdir,"%s","rpmpackage");
	sprintf(rpmdir,"%s%c%s", directory, DIRECTORY_SEPARATOR, subdir);
	if (directory_exists(rpmdir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,rpmdir);
		retval = system(commandstr);
	}

	save_spec(directory, rpmdir,
			  no_of_binaries, binaries);

	save_script(directory, subdir);

	return retval;
}
