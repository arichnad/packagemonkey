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

#include "makefile.h"

/* returns the row index of the given entry within a makefile */
int get_makefile_entry_from_file(char * makefilename, char * section, char * entry)
{
	int i, j, line_number = 0, is_line, begin_read = 0, index = -1;
	FILE * fp;
	char linestr[BLOCK_SIZE],linestr2[BLOCK_SIZE];

	/* check if the Makefile exists */
	if (file_exists(makefilename) == 0) return -1;

	fp = fopen(makefilename,"r");
	if (!fp) return -1;

	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			j = 0;
			is_line=0;
			for (i = 0; i < strlen(linestr); i++) {
				if (linestr[i] != '\t') {
					if ((linestr[i] != 10) &&
						(linestr[i] != 13)) {
						linestr2[j++] = linestr[i];
					}				
				}
				else {
					/* this is a line and not a section heading */
					is_line=1;
				}
			}
			linestr2[j]=0;
			if (is_line==0) {
				/* compare the section name */
				if (strncmp(linestr2,section,strlen(section))==0) {
					begin_read = 1;
				}
				else {
					if (begin_read == 1) {
						break;
					}
				}
			}
			else {
				/* compare the line */
				if (strcmp(linestr2,entry)==0) {
					index = line_number;
					break;
				}
			}
			line_number++;
		}
	}
	fclose(fp);

	return index;
}

/* returns the row index of the given entry */
int get_makefile_entry(char * section, char * entry)
{
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	/* get the project directory */
	get_setting("directory",directory);

	/* path and filename */
	sprintf(filename,"%s%cMakefile",directory,DIRECTORY_SEPARATOR);

	/* check if the Makefile exists */
	if (file_exists(filename) == 0) return -1;

	return get_makefile_entry_from_file(filename, section, entry);
}

/* adds an entry into a makefile */
int add_makefile_entry_to_file(char * makefilename, char * section, char * entry)
{
	int index,row=0,i,j,is_line,section_found=0,retval;
	char new_filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE], linestr2[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	FILE * fp, * fp_new;

	if (strlen(entry) == 0) return -1;

	index = get_makefile_entry_from_file(makefilename, section, entry);
	if (index != -1) return -1; /* entry already exists */

	sprintf(new_filename,"%s.new", makefilename);

	fp_new = fopen(new_filename,"w");
	if (!fp_new) return -1;
	
	fp = fopen(makefilename,"r");
	if (!fp) return -1;

	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;

			j = 0;
			is_line=0;
			for (i = 0; i < strlen(linestr); i++) {
				if (linestr[i] != '\t') {
					if ((linestr[i] != 10) &&
						(linestr[i] != 13)) {
						linestr2[j++] = linestr[i];
					}				
				}
				else {
					/* this is a line and not a section heading */
					is_line=1;
				}
			}
			linestr2[j]=0;

			if (is_line == 0) {
				switch(section_found) {
				case 0: {
					if (strstr(linestr2,section) != NULL) {
						section_found = 1;
					}
					break;
				}
				case 1: {
					if (section_found == 1) {
						if (entry[0] != '\t') {
							fprintf(fp_new,"\t%s\n",entry);
						}
						else {
							fprintf(fp_new,"%s\n",entry);
						}
						index = row;
						section_found = 2;
					}
					break;
				}
				}
			}

			fprintf(fp_new,"%s",linestr);
			row++;
		}
	}

	if (section_found == 1) {
		if (entry[0] != '\t') {
			fprintf(fp_new,"\t%s\n",entry);
		}
		else {
			fprintf(fp_new,"%s\n",entry);
		}
	}

	fclose(fp);
	fclose(fp_new);

	sprintf(commandstr,"%s %s %s",COMMAND_COPY,new_filename,makefilename);
	retval = system(commandstr);
	sprintf(commandstr,"%s %s",COMMAND_DELETE,new_filename);
	retval = system(commandstr);
	retval = index;
	return retval;
}

int add_makefile_entry(char * section, char * entry)
{
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	/* get the project directory */
	get_setting("directory",directory);

	/* path and filename */
	sprintf(filename,"%s%cMakefile",directory,DIRECTORY_SEPARATOR);

	/* check if the Makefile exists */
	if (file_exists(filename) == 0) return -1;

	return add_makefile_entry_to_file(filename, section, entry);
}

/* saves a makefile with a given filename */
void save_makefile_as(char * filename)
{
	char project_name[BLOCK_SIZE];
	char project_version[BLOCK_SIZE];
	char project_type[BLOCK_SIZE];
	FILE * fp;

	/* check if the Makefile exists */
	if (file_exists(filename) != 0) return;

	get_setting("project name",project_name);
	get_setting("project type",project_type);
	get_setting("version",project_version);

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"APP=%s\n",project_name);
	fprintf(fp,"VERSION=%s\n",project_version);
	fprintf(fp,"ARCH_TYPE=`uname -m`\n\n");

	fprintf(fp,"all:\n");
	if ((strcmp(project_type,"c")==0) ||
		(strcmp(project_type,"C")==0)) {
		fprintf(fp,"	gcc -Wall -std=gnu99 -pedantic -O3 -o $(APP) src/*.c -Isrc\n\n");
	}
	if ((strcmp(project_type,"c++")==0) ||
		(strcmp(project_type,"C++")==0) ||
		(strcmp(project_type,"cpp")==0) ||
		(strcmp(project_type,"CPP")==0)) {
		fprintf(fp,"	g++ -Wall -pedantic -O3 -o ${APP} src/*.cpp -Isrc\n\n");
	}
	fprintf(fp,"debug:\n");
	if ((strcmp(project_type,"c")==0) ||
		(strcmp(project_type,"C")==0)) {
		fprintf(fp,"	gcc -Wall -std=gnu99 -pedantic -g -o $(APP) src/*.c -Isrc\n\n");
	}
	if ((strcmp(project_type,"c++")==0) ||
		(strcmp(project_type,"C++")==0) ||
		(strcmp(project_type,"cpp")==0) ||
		(strcmp(project_type,"CPP")==0)) {
		fprintf(fp,"	g++ -Wall -pedantic -g -o ${APP} src/*.cpp -Isrc\n\n");
	}

	fprintf(fp,"source:\n");
	fprintf(fp,"	tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs\n\n");

	fprintf(fp,"install:\n");
	fprintf(fp,"	install -m 755 --strip $(APP) $(DESTDIR)/usr/bin\n");
	fprintf(fp,"	install -m 644 man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1\n");

	fprintf(fp,"	mkdir -m 755 -p /usr/share/applications\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/applications/$(APP)\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/pixmaps\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/icons\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/icons/hicolor\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/icons/hicolor/scalable\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/icons/hicolor/scalable/apps\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/icons/hicolor/24x24\n");
	fprintf(fp,"	mkdir -m 755 -p /usr/share/icons/hicolor/24x24/apps\n");
	fprintf(fp,"	install -m 644 desktop/$(APP).desktop /usr/share/applications/$(APP)/$(APP).desktop\n");
	fprintf(fp,"	install -m 644 desktop/icon24.png /usr/share/icons/hicolor/24x24/apps/$(APP).png\n");
	fprintf(fp,"	install -m 644 desktop/icon.svg /usr/share/icons/hicolor/scalable/apps/$(APP).svg\n");
	fprintf(fp,"	install -m 644 desktop/icon.svg /usr/share/pixmaps/$(APP).svg\n\n");


	fprintf(fp,"clean:\n");
	fprintf(fp,"	rm -f $(APP) \\#* \\.#* gnuplot* *.png debian/*.substvars debian/*.log\n");
	fprintf(fp,"	rm -rf deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)\n");
	fprintf(fp,"	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc\n");
	fprintf(fp,"	rm -f rpmpackage/*.src.rpm\n");

	fclose(fp);
}

/* saves a makefile */
void save_makefile()
{
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	/* get the project directory */
	get_setting("directory",directory);

	/* path and filename */
	sprintf(filename,"%s%cMakefile",directory,DIRECTORY_SEPARATOR);

	save_makefile_as(filename);
}
