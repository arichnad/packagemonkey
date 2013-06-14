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
int get_makefile_entry(char * section, char * entry)
{
	int i, j, line_number = 0, is_line, begin_read = 0, index = -1;
	FILE * fp;
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE],linestr2[BLOCK_SIZE];

	/* get the project directory */
	get_setting("directory",directory);

	/* path and filename */
	sprintf(filename,"%s%cMakefile",directory,DIRECTORY_SEPARATOR);

	/* check if the Makefile exists */
	if (file_exists(filename) == 0) return -1;

	fp = fopen(filename,"r");
	if (!fp) return -1;

	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			j = 0;
			is_line=0;
			for (i = 0; i < strlen(linestr); i++) {
				if ((linestr[i] != '\t') &&
					(linestr[i] != 10) &&
					(linestr[i] != 13)) {
					linestr2[j++] = linestr[i];
				}
				else {
					is_line=1;
				}
			}
			linestr2[j]=0;
			if (is_line==0) {
				/* compare the section name */
				if (strncmp(linestr2,section,strlen(section))==0) {
					begin_read=1;
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

int add_makefile_entry(char * section, char * entry)
{
	return 0;
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
	fprintf(fp,"	cp $(APP) $(DESTDIR)/usr/bin\n");
	fprintf(fp,"	cp man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1\n");
	fprintf(fp,"	chmod 755 $(DESTDIR)/usr/bin/$(APP)\n");
	fprintf(fp,"	chmod 644 $(DESTDIR)/usr/share/man/man1/$(APP).1.gz\n\n");

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
