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

/* returns 1 if the given makefile section is empty */
int empty_makefile_section(char * makefilename,
						   char * section)
{
	int i, j, line_number = 0, is_line, line_count=0;
	int begin_read = 0;
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
				if (strncmp(linestr2,section,
							strlen(section))==0) {
					begin_read = 1;
				}
				else {
					if (begin_read == 1) {
						break;
					}
				}
			}
			else {
				if (begin_read == 1) {
					/* does the line contain some characters? */
					if (strlen(linestr2) > 0) {
						line_count++;
					}
					break;
				}
			}
			line_number++;
		}
	}
	fclose(fp);

	if (line_count > 0) {
		return 0;
	}
	return 1;
}


/* returns the row index of the given entry within a makefile */
int get_makefile_entry_from_file(char * makefilename,
								 char * section, char * entry)
{
	int i, j, line_number = 0, is_line;
	int begin_read = 0, index = -1;
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
				if (strncmp(linestr2, section,
							strlen(section)) == 0) {
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
				if (begin_read == 1) {
					if (strcmp(linestr2, entry) == 0) {
						index = line_number;
						break;
					}
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
	sprintf(filename,"%s%cMakefile",directory,
			DIRECTORY_SEPARATOR);

	/* check if the Makefile exists */
	if (file_exists(filename) == 0) return -1;

	return get_makefile_entry_from_file(filename, section, entry);
}

/* adds an entry into a makefile */
int add_makefile_entry_to_file(char * makefilename,
							   char * section, char * entry)
{
	int index,row=0,i,j,is_line,section_found=0,retval;
	char new_filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE], linestr2[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	FILE * fp, * fp_new;

	if (strlen(entry) == 0) return -1;

	index = get_makefile_entry_from_file(makefilename,
										 section, entry);
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

	if (section_found == 0) {
		/* no section was found, so create one */
		fprintf(fp_new,"\n%s:\n",section);
		fprintf(fp_new,"\t%s\n",entry);
	}

	fclose(fp);
	fclose(fp_new);

	sprintf(commandstr,"%s %s %s",COMMAND_COPY,
			new_filename,makefilename);
	retval = system(commandstr);
	sprintf(commandstr,"%s %s",COMMAND_DELETE,new_filename);
	retval = system(commandstr);
	retval = index;
	return retval;
}

/* adds an entry to a makefile section */
int add_makefile_entry(char * section, char * entry)
{
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	/* get the project directory */
	get_setting("directory",directory);

	/* path and filename */
	sprintf(filename,"%s%cMakefile",directory,
			DIRECTORY_SEPARATOR);

	/* check if the Makefile exists */
	if (file_exists(filename) == 0) return -1;

	return add_makefile_entry_to_file(filename, section, entry);
}

/* saves a makefile with a given filename */
void save_makefile_as(char * filename)
{
	char project_name[BLOCK_SIZE];
	char project_version[BLOCK_SIZE];
	FILE * fp;

	/* check if the Makefile exists */
	if (file_exists(filename) != 0) return;

	get_setting("project name",project_name);
	get_setting("version",project_version);

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"APP=%s\n",project_name);
	fprintf(fp,"VERSION=%s\n",project_version);
	fprintf(fp,"%s","RELEASE=1\n");
	fprintf(fp,"ARCH_TYPE=`uname -m`\n\n");

	fprintf(fp,"all:\n");

	fprintf(fp,"debug:\n");

	fprintf(fp,"source:\n");

	fprintf(fp,"install:\n");

	fprintf(fp,"clean:\n");

	fclose(fp);
}

/* creates a dummy configure file */
int save_configure(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];

	sprintf(filename,"%s%cconfigure",
			directory, DIRECTORY_SEPARATOR);

	fp = fopen(filename,"w");
	if (!fp) return 0;
	fprintf(fp,"%s", " ");
	fclose(fp);

	sprintf(commandstr,"chmod +x %s", filename);
	return system(commandstr);
}

/* saves a makefile */
void save_makefile(int no_of_binaries, char ** binaries)
{
	int i,j,no_of_directories;
	char directory[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	char str[BLOCK_SIZE];
	char svg_filename[BLOCK_SIZE];
	char project_type[BLOCK_SIZE];
	char project_version[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char compile_args[BLOCK_SIZE];
	char commandline[BLOCK_SIZE];
	char * directories[MAX_FILES];

	/* get the project directory */
	get_setting("directory",directory);

	/* the type of project */
	get_setting("project type",project_type);

	/* the project name */
	get_setting("project name",project_name);

	/* the project version */
	get_setting("version",project_version);

	/* compiler arguments */
	get_setting("compile",compile_args);

	/* command line project */
	get_setting("commandline",commandline);

	/* path and filename */
	sprintf(filename,"%s%cMakefile",directory,
			DIRECTORY_SEPARATOR);

	save_makefile_as(filename);

	/* add lines to the makefile if they don't exist */
	add_makefile_entry_to_file(filename, "source",
							   "tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs");

	add_makefile_entry_to_file(filename, "install",
							   "mkdir -p $(DESTDIR)/usr");
	add_makefile_entry_to_file(filename, "install",
							   "mkdir -p $(DESTDIR)/usr/bin");

	if (is_library(project_name) != 0) {
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -p $(DESTDIR)/usr/lib");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -p $(DESTDIR)/usr/lib/$(APP)");
	}

	/* create directories for binaries */
	if (no_of_binaries > 0) {
		no_of_directories =
			get_directories(binaries, no_of_binaries,
							directories);
		for (i = 0; i < no_of_directories; i++) {
			if (get_subdirectory_string(directories[i]) != 0) {
				if (contains_char(get_subdirectory_string(directories[i]),' ')==0) {
					sprintf(str,"mkdir -m 755 -p $(DESTDIR)/%s",
							get_subdirectory_string(directories[i]));
				}
				else {
					sprintf(str,"mkdir -m 755 -p $(DESTDIR)\"/%s\"",
							get_subdirectory_string(directories[i]));
				}
				add_makefile_entry_to_file(filename, "install",
										   str);
			}
			free(directories[i]);
		}
	}

	if ((strcmp(project_type,"c")==0) ||
		(strcmp(project_type,"C")==0) ||
		(strcmp(project_type,"c++")==0) ||
		(strcmp(project_type,"C++")==0) ||
		(strcmp(project_type,"cpp")==0) ||
		(strcmp(project_type,"CPP")==0)) {
		add_makefile_entry_to_file(filename, "install",
								   "install -m 755 --strip $(APP) $(DESTDIR)/usr/bin");
	}

	if (is_library(project_name) == 0) {
		/* install binary files from a directory */
		for (i = 0; i < no_of_binaries; i++) {
			if (get_subdirectory_string(binaries[i]) != 0){
				if (contains_char(binaries[i],' ')==0) {
					sprintf(str,"install -m 755 %s $(DESTDIR)/%s",
							binaries[i],
							get_subdirectory_string(binaries[i]));
				}
				else {
					sprintf(str,"install -m 755 \"%s\" $(DESTDIR)\"/%s\"",
							binaries[i],
							get_subdirectory_string(binaries[i]));
				}
				add_makefile_entry_to_file(filename, "install", str);
			}
		}
	}
	else {	
		for (i = 0; i < no_of_binaries; i++) {
			for (j=strlen(binaries[i])-1; j>=0; j--) {
				if (binaries[i][j] == DIRECTORY_SEPARATOR) {
					j++;
					break;
				}
			}
			if (contains_char(binaries[i],' ')==0) {
				sprintf(str,"install -m 755 %s " \
						"$(DESTDIR)/usr/lib/$(APP)/%s",
						binaries[i], &binaries[i][j]);
			}
			else {
				sprintf(str,"install -m 755 \"%s\" " \
						"$(DESTDIR)\"/usr/lib/$(APP)/%s\"",
						binaries[i], &binaries[i][j]);
			}
			add_makefile_entry_to_file(filename, "install", str);

			sprintf(str,"ln -sf $(DESTDIR)/usr/lib/%s.0.0.$(RELEASE) " \
					"$(DESTDIR)/usr/lib/%s.so.0",
					&binaries[i][j], &binaries[i][j]);
			add_makefile_entry_to_file(filename, "install",str);

			sprintf(str,"ln -sf $(DESTDIR)/usr/lib/%s.0.0.$(RELEASE) " \
					"$(DESTDIR)/usr/lib/$(APP)/%s.so",
					&binaries[i][j], &binaries[i][j]);
			add_makefile_entry_to_file(filename, "install",str);
		}
		add_makefile_entry_to_file(filename, "install","ldconfig");
	}
	add_makefile_entry_to_file(filename, "install",
							   "mkdir -m 755 -p $(DESTDIR)/usr/share");
	add_makefile_entry_to_file(filename, "install",
							   "mkdir -m 755 -p $(DESTDIR)/usr/share/man");
	add_makefile_entry_to_file(filename, "install",
							   "mkdir -m 755 -p $(DESTDIR)/usr/share/man/man1");
	add_makefile_entry_to_file(filename, "install",
							   "install -m 644 man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1");

	/* additional install for desktop icons */
	if (strlen(commandline) == 0) {
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/$(APP)");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/applications");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/pixmaps");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/icons");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/scalable");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/24x24");
		add_makefile_entry_to_file(filename, "install",
								   "mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/24x24/apps");
		add_makefile_entry_to_file(filename, "install",
								   "install -m 644 desktop/$(APP).desktop $(DESTDIR)/usr/share/applications/$(APP).desktop");
		add_makefile_entry_to_file(filename, "install",
								   "install -m 644 desktop/icon24.png $(DESTDIR)/usr/share/icons/hicolor/24x24/apps/$(APP).png");
		sprintf(svg_filename,"%s%cdesktop%cicon.svg", directory, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);
		if (file_exists(svg_filename) != 0) {
			add_makefile_entry_to_file(filename, "install",
									   "install -m 644 desktop/icon.svg $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/$(APP).svg");
			add_makefile_entry_to_file(filename, "install",
									   "install -m 644 desktop/icon.svg $(DESTDIR)/usr/share/pixmaps/$(APP).svg");
		}
	}

	if ((strcmp(project_type,"c")==0) ||
		(strcmp(project_type,"C")==0)) {
		if (empty_makefile_section(filename,"all") == 1) {
			sprintf(str, "gcc -Wall -std=gnu99 -pedantic " \
					"-O3 -o $(APP) src/*.c -Isrc %s", compile_args);
			add_makefile_entry_to_file(filename, "all", str);
		}
		if (empty_makefile_section(filename,"debug") == 1) {
			sprintf(str, "gcc -Wall -std=gnu99 -pedantic " \
					"-g -o $(APP) src/*.c -Isrc %s", compile_args);
			add_makefile_entry_to_file(filename, "debug", str);
		}
	}
	if ((strcmp(project_type,"c++")==0) ||
		(strcmp(project_type,"C++")==0) ||
		(strcmp(project_type,"cpp")==0) ||
		(strcmp(project_type,"CPP")==0)) {
		if (empty_makefile_section(filename,"all") == 1) {
			sprintf(str, "g++ -Wall -pedantic -O3 "	\
					"-o ${APP} src/*.cpp -Isrc %s", compile_args);
			add_makefile_entry_to_file(filename, "all", str);
		}
		if (empty_makefile_section(filename,"debug") == 1) {
			sprintf(str, "g++ -Wall -pedantic -g " \
					"-o ${APP} src/*.cpp -Isrc %s", compile_args);
			add_makefile_entry_to_file(filename, "debug", str);
		}
	}

	sprintf(str,"%s $(APP) \\#* \\.#* gnuplot* " \
			"*.png %s/*.substvars %s/*.log",
			COMMAND_DELETE, DEB_SUBDIR, DEB_SUBDIR);
	add_makefile_entry_to_file(filename, "clean", str);

	sprintf(str,"%sr deb.* %s/$(APP) %s/$(ARCH_TYPE)",
			COMMAND_DELETE, DEB_SUBDIR, RPM_SUBDIR);
	add_makefile_entry_to_file(filename, "clean", str);
	add_makefile_entry_to_file(filename, "clean",
							   "rm -f ../$(APP)*.deb " \
							   "../$(APP)*.changes " \
							   "../$(APP)*.asc " \
							   "../$(APP)*.dsc");
	sprintf(str,"%s %s/*.src.rpm %s/*.gz %s/*.gz %s/*.pet",
			COMMAND_DELETE, RPM_SUBDIR, ARCH_SUBDIR,
			PUPPY_SUBDIR, PUPPY_SUBDIR);
	add_makefile_entry_to_file(filename, "clean", str);

	replace_build_script_version(filename,
								 project_name,
								 project_version);
}
