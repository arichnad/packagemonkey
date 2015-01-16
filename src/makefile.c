/*
  packagemonkey - a package creation assistant
  Copyright (C) 2013-2015  Bob Mottram <bob@robotics.uk.to>

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

/* Returns a non-zero value if this is a QT project.
   If this is a QT project then a file called src/projectname.pro should exist,
   where "projectname" is the name of the project */
static int is_qt_project()
{
    char directory[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];
    char filename[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];

    get_setting("project type", project_type);
    if (strcmp(project_type, "cpp") == 0) {
        get_setting("directory", directory);
        get_setting("source dir", sourcedir);
        get_setting("project name", project_name);

        sprintf(filename, "%s%c%s%c%s.pro",
                directory, DIRECTORY_SEPARATOR, sourcedir,
                DIRECTORY_SEPARATOR, project_name);

        if (file_exists(filename) != 0) {
            return 1;
        }
    }
    return 0;
}

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
    char linestr[BLOCK_SIZE], entry_line[BLOCK_SIZE];
    char makefile_line[BLOCK_SIZE], sectionstr[BLOCK_SIZE];

    /* section name with a colon */
    sprintf(sectionstr, "%s:", section);

    /* check if the Makefile exists */
    if (file_exists(makefilename) == 0) return -1;

    fp = fopen(makefilename,"r");
    if (!fp) return -1;

    /* remove any stray characters from the entry */
    j = 0;
    for (i = 0; i < strlen(entry); i++) {
        if ((entry[i] != '\t') &&
            (entry[i] != 10) &&
            (entry[i] != 13)) {
            entry_line[j++] = entry[i];
        }
    }
    entry_line[j] = 0;

    while (!feof(fp)) {
        if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
            if (strlen(linestr) == 0) continue;
            j = 0;
            is_line = 0;
            for (i = 0; i < strlen(linestr); i++) {
                if (linestr[i] != '\t') {
                    if ((linestr[i] != 10) &&
                        (linestr[i] != 13)) {
                        makefile_line[j++] = linestr[i];
                    }
                }
                else {
                    /* this is a line and not a section heading */
                    is_line = 1;
                }
            }
            makefile_line[j] = 0;
            if (is_line == 0) {
                /* compare the section name */
                if (strncmp(makefile_line, sectionstr,
                            strlen(sectionstr)) == 0) {
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
                    if (strcmp(makefile_line, entry_line) == 0) {
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

/* saves a skeletal makefile with a given filename */
void save_makefile_as(char * filename)
{
    char project_name[BLOCK_SIZE];
    char project_version[BLOCK_SIZE];
    FILE * fp;

    /* check if the Makefile exists */
    if (file_exists(filename) != 0) return;

    get_setting("project name", project_name);
    get_setting("version", project_version);

    fp = fopen(filename, "w");
    if (!fp) return;

    fprintf(fp, "APP=%s\n", project_name);
    fprintf(fp, "VERSION=%s\n", project_version);
    fprintf(fp, "%s", "RELEASE=1\n");

    if (is_library(project_name) != 0) {
        fprintf(fp, "%s", "SONAME=${APP}.so.0\n");
        fprintf(fp, "%s", "LIBNAME=${APP}-${VERSION}.so.0.0.${RELEASE}\n");
    }

    fprintf(fp, "%s", "ARCH_TYPE=`uname -m`\n");
    fprintf(fp, "%s", "PREFIX?=/usr/local\n");
    fprintf(fp, "%s", "LIBDIR=lib\n\n");

    fprintf(fp, "%s", "ifeq ($(shell if [ -d /usr/lib64 ]; then echo \"found\"; fi;), \"found\")\n");
    fprintf(fp, "%s", "LIBDIR = lib64\n");
    fprintf(fp, "%s", "endif\n");

    /* if this is a QT project then create a build directory
       for the GUI */
    if (is_qt_project() != 0) {
        fprintf(fp, "%s", "CURDIR := $(shell pwd)\n");
        fprintf(fp, "%s", "GUI_DIR := $(CURDIR)/build\n");
        fprintf(fp, "%s", "$(shell [ -d \"$(GUI_DIR)\" ] || mkdir -p $(GUI_DIR))\n\n");
    }

    fprintf(fp,"all:\n");

    fprintf(fp,"debug:\n");

    fprintf(fp,"source:\n");

    fprintf(fp,"install:\n");

    fprintf(fp,"uninstall:\n");

    if (is_library(project_name) != 0) {
        fprintf(fp,"instlib:\n");
    }

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

    if (file_exists(filename) != 0) return 0;

    /* dummy file containing a single space character */
    fp = fopen(filename,"w");
    if (!fp) return 0;
    fprintf(fp,"%s", " ");
    fclose(fp);

    sprintf(commandstr,"chmod +x %s", filename);
    return system(commandstr);
}

/* if the project is written in a scripted language and has a main script
   specified then copy the scripts to the relevant location */
static void save_makefile_install_scripts(char * filename,
                                          char * section)
{
    char project_type[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char mainscript[BLOCK_SIZE];
    char str[BLOCK_SIZE];
    char runscript[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];
    char library_path[BLOCK_SIZE];
    char progname[BLOCK_SIZE];
    char python_version[BLOCK_SIZE];

    get_setting("project type", project_type);
    get_setting("project name", project_name);
    get_setting("main script", mainscript);
    get_setting("source dir", sourcedir);
    get_setting("library path", library_path);

    /* only applies to executable applications */
    if (is_library(project_name) != 0) return;

    /* a main script must have been specified */
    if (strlen(mainscript) == 0) return;

    /* if this is not a script language */
    if (is_script_language(project_type) == 0) return;

    /* ensure that there is a share directory */
    add_makefile_entry_to_file(filename, section,
                               "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share");

    /* ensure that there is an application directory */
    add_makefile_entry_to_file(filename, section,
                               "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/${APP}");

    /* copy script files into the application directory */
    sprintf(str,"%s -rf %s/* ${DESTDIR}${PREFIX}/share/${APP}",
            COMMAND_COPY, sourcedir);
    add_makefile_entry_to_file(filename, section, str);

    /* name of the run script */
    sprintf(runscript, "${DESTDIR}${PREFIX}/bin/${APP}");

    /* create a run script */
    sprintf(str, "echo '#!/bin/sh' > %s", runscript);
    add_makefile_entry_to_file(filename, section, str);

    /* if installed to the local directory */
    sprintf(str, "echo 'if [ -d ${DESTDIR}${PREFIX}/share/${APP} ]; then' >> %s",
            runscript);
    add_makefile_entry_to_file(filename, section, str);

    /* move to the project directory */
    sprintf(str, "echo '  cd ${DESTDIR}${PREFIX}/share/${APP}' >> %s",
            runscript);
    add_makefile_entry_to_file(filename, section, str);

    /* else */
    sprintf(str, "echo 'else' >> %s", runscript);
    add_makefile_entry_to_file(filename, section, str);

    sprintf(str, "echo '  cd /usr/share/${APP}' >> %s",
            runscript);
    add_makefile_entry_to_file(filename, section, str);

    /* endif */
    sprintf(str, "echo 'fi' >> %s", runscript);
    add_makefile_entry_to_file(filename, section, str);


    /* run the main script */
    if (is_script_language(project_type) != 0) {
        sprintf(progname,"%s","");
        if (strcmp(project_type,"pl") == 0) {
            sprintf(progname,"%s","perl");
        }
        if (strcmp(project_type,"py") == 0) {
            get_setting("python version", python_version);
            if (strlen(python_version) > 0) {
                if (strlen(python_version) == 1) {
                    sprintf(progname,
                            "python%s", python_version);
                }
                else {
                    sprintf(progname, "%s",
                            python_version);
                }
            }
            else {
                sprintf(progname,"%s","python");
            }
        }
        if (strlen(progname) > 0) {
            if (strlen(library_path) > 0) {
                /* add library path */
                sprintf(str,
                        "echo 'LD_LIBRARY_PATH=\"%s\" " \
                        "exec %s %s' >> %s",
                        library_path, progname,
                        mainscript, runscript);
            }
            else {
                /* just run the script */
                sprintf(str, "echo 'exec %s %s' >> %s",
                        progname, mainscript, runscript);
            }
            add_makefile_entry_to_file(filename, section, str);
        }
    }
    if (strcmp(project_type,"pl") == 0) {
        if (strlen(library_path) > 0) {
            /* add library path */
            sprintf(str,
                    "echo 'LD_LIBRARY_PATH=\"%s\" " \
                    "exec perl %s' >> %s",
                    library_path, mainscript, runscript);
        }
        else {
            /* just run the script */
            sprintf(str, "echo 'exec perl %s' >> %s",
                    mainscript, runscript);
        }
        add_makefile_entry_to_file(filename, section, str);
    }

    /* make the script runable */
    sprintf(str, "chmod +x %s", runscript);
    add_makefile_entry_to_file(filename, section, str);
}

/* instructions to uninstall */
static void save_makefile_uninstall(char * filename,
                                    char * project_name,
                                    char * commandline,
                                    int no_of_binaries, char ** binaries)
{
    int i;
    char str[BLOCK_SIZE];

    /* remove any installed binaries */
    for (i = 0; i < no_of_binaries; i++) {
        if (get_subdirectory_string(binaries[i]) != 0){
            if (contains_char(binaries[i],' ')==0) {
                sprintf(str,"rm -f /%s",
                        get_subdirectory_string(binaries[i]));
            }
            else {
                sprintf(str,"rm -f \"/%s\"",
                        get_subdirectory_string(binaries[i]));
            }
            add_makefile_entry_to_file(filename, "uninstall", str);
        }
    }

    /* remove the manpage */
    add_makefile_entry_to_file(filename, "uninstall",
                               "rm -f ${PREFIX}/share/man/man1/${APP}.1.gz");

    if (is_library(project_name) == 0) {
        /* remove the application */
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -rf ${PREFIX}/share/${APP}");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/bin/${APP}");
    }
    else {
        /* remove the library */
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/${LIBDIR}/${LIBNAME}");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/${LIBDIR}/${APP}.so");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/${LIBDIR}/${SONAME}");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -rf ${PREFIX}/include/${APP}");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "ldconfig");
    }

    if (strlen(commandline) == 0) {
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/share/applications/${APP}.desktop");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/share/icons/hicolor/scalable/apps/${APP}.svg");
        add_makefile_entry_to_file(filename, "uninstall",
                                   "rm -f ${PREFIX}/share/pixmaps/${APP}.svg");
    }
}

/* saves the install section of a makefile */
void save_makefile_install(char * filename,
                           char * section,
                           int no_of_binaries, char ** binaries,
                           char * project_name,
                           char * project_type,
                           char * commandline,
                           char * directory,
                           char * svg_filename)
{
    int i, no_of_directories;
    char str[BLOCK_SIZE];
    char * directories[MAX_FILES];
    char sourcedir[BLOCK_SIZE];
    int is_install_lib = 0;

    get_setting("source dir", sourcedir);

    if (strcmp(section,"install") != 0) {
        is_install_lib = 1;
    }

    add_makefile_entry_to_file(filename, section,
                               "mkdir -p ${DESTDIR}/usr");

    add_makefile_entry_to_file(filename, section,
                               "mkdir -p ${DESTDIR}${PREFIX}");

    if (is_library(project_name) != 0) {
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -p ${DESTDIR}${PREFIX}/${LIBDIR}");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -p ${DESTDIR}${PREFIX}/${LIBDIR}/${APP}");
    }
    else {
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -p ${DESTDIR}${PREFIX}/bin");
    }

    /* create directories for binaries */
    if (no_of_binaries > 0) {
        no_of_directories =
            get_directories(binaries, no_of_binaries,
                            directories);
        for (i = 0; i < no_of_directories; i++) {
            if (get_subdirectory_string(directories[i]) != 0) {
                if (contains_char(get_subdirectory_string(directories[i]),' ')==0) {
                    sprintf(str,"mkdir -m 755 -p ${DESTDIR}/%s",
                            get_subdirectory_string(directories[i]));
                }
                else {
                    sprintf(str,"mkdir -m 755 -p ${DESTDIR}\"/%s\"",
                            get_subdirectory_string(directories[i]));
                }
                add_makefile_entry_to_file(filename, section, str);
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
        if (is_library(project_name) == 0) {
            if (is_qt_project() != 0) {
                /* QT executable */
                add_makefile_entry_to_file(filename, section,
                                           "install -m 755 -s ${GUI_DIR}/${APP} " \
                                           "${DESTDIR}${PREFIX}/bin");
            }
            else {
                /* executable */
                add_makefile_entry_to_file(filename, section,
                                           "install -m 755 -s ${APP} " \
                                           "${DESTDIR}${PREFIX}/bin");
            }
        }
        else {
            /* header files */
            sprintf(str, "%s -p ${DESTDIR}${PREFIX}/include", COMMAND_MKDIR);
            add_makefile_entry_to_file(filename, section, str);

            sprintf(str, "%s -p ${DESTDIR}${PREFIX}/include/${APP}", COMMAND_MKDIR);
            add_makefile_entry_to_file(filename, section, str);

            sprintf(str,"%s %s/*.h ${DESTDIR}${PREFIX}/include/${APP}",
                    COMMAND_COPY, sourcedir);
            add_makefile_entry_to_file(filename, section, str);

            /* library */
            add_makefile_entry_to_file(filename, section,
                                       "install -m 755 ${LIBNAME} " \
                                       "${DESTDIR}${PREFIX}/${LIBDIR}");

            if (is_install_lib == 0) {
                add_makefile_entry_to_file(filename, section,
                                           "ln -sf ${DESTDIR}${PREFIX}/${LIBDIR}/${LIBNAME} " \
                                           "${DESTDIR}${PREFIX}/${LIBDIR}/${SONAME}");
                add_makefile_entry_to_file(filename, section,
                                           "ln -sf ${DESTDIR}${PREFIX}/${LIBDIR}/${LIBNAME} " \
                                           "${DESTDIR}${PREFIX}/${LIBDIR}/${APP}.so");
            }
        }
    }

    /* install binary files from a directory */
    for (i = 0; i < no_of_binaries; i++) {
        if (get_subdirectory_string(binaries[i]) != 0){
            if (contains_char(binaries[i],' ')==0) {
                sprintf(str,"install -m 755 %s ${DESTDIR}/%s",
                        binaries[i],
                        get_subdirectory_string(binaries[i]));
            }
            else {
                sprintf(str,"install -m 755 \"%s\" ${DESTDIR}\"/%s\"",
                        binaries[i],
                        get_subdirectory_string(binaries[i]));
            }
            add_makefile_entry_to_file(filename, section, str);
        }
    }

    if ((is_library(project_name) != 0) &&
        (is_install_lib == 0)) {
        add_makefile_entry_to_file(filename, section,"ldconfig");
    }

    /* install the manpage */
    add_makefile_entry_to_file(filename, section,
                               "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share");
    add_makefile_entry_to_file(filename, section,
                               "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man");
    add_makefile_entry_to_file(filename, section,
                               "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1");
    add_makefile_entry_to_file(filename, section,
                               "install -m 644 man/${APP}.1.gz " \
                               "${DESTDIR}${PREFIX}/share/man/man1");

    /* additional install for desktop icons */
    if (strlen(commandline) == 0) { /* not a commandline project */
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/${APP}");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/" \
                                   "applications");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/pixmaps");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/icons");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/" \
                                   "icons/hicolor");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/" \
                                   "icons/hicolor/scalable");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/" \
                                   "icons/hicolor/scalable/apps");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/" \
                                   "icons/hicolor/24x24");
        add_makefile_entry_to_file(filename, section,
                                   "mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/" \
                                   "icons/hicolor/24x24/apps");
        add_makefile_entry_to_file(filename, section,
                                   "install -m 644 desktop/${APP}.desktop " \
                                   "${DESTDIR}${PREFIX}/share/applications/" \
                                   "${APP}.desktop");
        add_makefile_entry_to_file(filename, section,
                                   "install -m 644 desktop/icon24.png " \
                                   "${DESTDIR}${PREFIX}/share/icons/hicolor/" \
                                   "24x24/apps/${APP}.png");
        sprintf(svg_filename,"%s%cdesktop%cicon.svg",
                directory, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);

        /* check for the existence of an SVG icon */
        if (file_exists(svg_filename) != 0) {
            add_makefile_entry_to_file(filename, section,
                                       "install -m 644 desktop/icon.svg " \
                                       "${DESTDIR}${PREFIX}/share/icons/hicolor/" \
                                       "scalable/apps/${APP}.svg");
            add_makefile_entry_to_file(filename, section,
                                       "install -m 644 desktop/icon.svg " \
                                       "${DESTDIR}${PREFIX}/share/pixmaps/${APP}.svg");
        }
    }

    /* save any scripts to the appropriate locations */
    save_makefile_install_scripts(filename, section);
}

/* saves Scala compile instructions to a makefile */
static void save_makefile_scala(char * filename)
{
    char str[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char compile_args[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];
    char build_dir[BLOCK_SIZE];

    /* the project name */
    get_setting("project name", project_name);

    /* the type of project */
    get_setting("project type", project_type);

    /* compiler arguments */
    get_setting("compile", compile_args);

    /* get the directory within which the source code is located */
    get_setting("source dir", sourcedir);

    /* directory where results of compilation are held */
    get_setting("build dir", build_dir);

    if (strcmp(project_type,"scala")==0) {
        if (is_library(project_name) == 0) {
            /* compile an executable */
            if (empty_makefile_section(filename,"all") == 1) {
                sprintf(str, "scalac %s%c*.scala %s -sourcepath %s -d %s",
                        sourcedir, DIRECTORY_SEPARATOR,
                        compile_args, sourcedir, build_dir);
                add_makefile_entry_to_file(filename, "all", str);
            }
            if (empty_makefile_section(filename,"debug") == 1) {
                sprintf(str, "scalac -g %s%c*.scala %s -sourcepath %s -d %s",
                        sourcedir, DIRECTORY_SEPARATOR,
                        compile_args, sourcedir, build_dir);
                add_makefile_entry_to_file(filename, "debug", str);
            }
        }
        else {
            /* TODO: compile a shared library */
        }
    }
}

/* saves Vala compile instructions to a makefile */
static void save_makefile_vala(char * filename)
{
    char str[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char compile_args[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];

    /* the project name */
    get_setting("project name", project_name);

    /* the type of project */
    get_setting("project type", project_type);

    /* compiler arguments */
    get_setting("compile", compile_args);

    /* get the directory within which the source code is located */
    get_setting("source dir", sourcedir);

    if (strcmp(project_type,"vala")==0) {
        if (is_library(project_name) == 0) {
            /* compile an executable */
            if (empty_makefile_section(filename,"all") == 1) {
                sprintf(str, "valac %s%c*.vala %s --basedir %s -o ${APP}",
                        sourcedir, DIRECTORY_SEPARATOR,
                        compile_args, sourcedir);
                add_makefile_entry_to_file(filename, "all", str);
            }
            if (empty_makefile_section(filename,"debug") == 1) {
                sprintf(str, "valac -g --save-temps %s%c*.vala %s " \
                        "--basedir %s -o ${APP}",
                        sourcedir, DIRECTORY_SEPARATOR,
                        compile_args, sourcedir);
                add_makefile_entry_to_file(filename, "debug", str);
            }
        }
        else {
            /* TODO: compile a shared library */
        }
    }
}

/* saves C++ compile instructions to a makefile */
static void save_makefile_cpp(char * filename)
{
    char str[BLOCK_SIZE];
    char c_standard[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char compile_args[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];

    /* the project name */
    get_setting("project name", project_name);

    /* the type of project */
    get_setting("project type", project_type);

    /* the standard to be used by gcc/g++ */
    get_setting("c standard", c_standard);
    if (strcmp(c_standard, DEFAULT_C_STANDARD)==0) {
        sprintf(c_standard, "%s",
                DEFAULT_CPP_STANDARD);
    }

    /* compiler arguments */
    get_setting("compile", compile_args);

    /* get the directory within which the source code is located */
    get_setting("source dir", sourcedir);

    if ((strcmp(project_type,"c++")==0) ||
        (strcmp(project_type,"C++")==0) ||
        (strcmp(project_type,"cpp")==0) ||
        (strcmp(project_type,"CPP")==0)) {

        if (is_library(project_name) == 0) {
            /* compile an executable */

            if (is_qt_project() != 0) {
                /* if this is a QT project then create a Makefile
                   using qmake */
                sprintf(str, "%s",
                        "qmake ${CURDIR}/src/${APP}.pro -o " \
                        "${GUI_DIR}/Makefile");
                add_makefile_entry_to_file(filename, "all", str);
                add_makefile_entry_to_file(filename, "debug", str);

                sprintf(str, "%s", "$(MAKE) -C $(GUI_DIR)");
                add_makefile_entry_to_file(filename, "all", str);

                sprintf(str, "%s", "$(MAKE) -C $(GUI_DIR)/ debug");
                add_makefile_entry_to_file(filename, "debug", str);
            }
            else {
                if (empty_makefile_section(filename,"all") == 1) {
                    sprintf(str, "g++ -Wall -pedantic -O3 -std=%s " \
                            "-o ${APP} %s/*.cpp -I%s %s",
                            c_standard, sourcedir, sourcedir, compile_args);
                    add_makefile_entry_to_file(filename, "all", str);
                }
                if (empty_makefile_section(filename,"debug") == 1) {
                    sprintf(str, "g++ -Wall -pedantic -g -std=%s " \
                            "-o ${APP} %s/*.cpp -I%s %s",
                            c_standard, sourcedir, sourcedir, compile_args);
                    add_makefile_entry_to_file(filename, "debug", str);
                }
            }
        }
        else {
            /* compile a shared library */
            if (empty_makefile_section(filename,"all") == 1) {
                sprintf(str, "g++ -shared -Wl,-soname,${SONAME} " \
                        "-pedantic -fPIC -O3 -std=%s " \
                        "-o ${LIBNAME} %s/*.cpp -I%s %s",
                        c_standard, sourcedir, sourcedir, compile_args);
                add_makefile_entry_to_file(filename, "all", str);
            }
            if (empty_makefile_section(filename,"debug") == 1) {
                sprintf(str, "g++ -shared -Wl,-soname,${SONAME} " \
                        "-pedantic -fPIC -g -std=%s " \
                        "-o ${LIBNAME} %s/*.cpp -I%s %s",
                        c_standard, sourcedir, sourcedir, compile_args);
                add_makefile_entry_to_file(filename, "debug", str);
            }
        }
    }
}

/* saves C compile instructions to a makefile */
static void save_makefile_c(char * filename)
{
    char str[BLOCK_SIZE];
    char c_standard[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char compile_args[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];

    /* the project name */
    get_setting("project name", project_name);

    /* the type of project */
    get_setting("project type", project_type);

    /* the standard to be used by gcc/g++ */
    get_setting("c standard", c_standard);

    /* compiler arguments */
    get_setting("compile", compile_args);

    /* get the directory within which the source code is located */
    get_setting("source dir", sourcedir);

    if ((strcmp(project_type,"c")==0) ||
        (strcmp(project_type,"C")==0)) {
        if (is_library(project_name) == 0) {
            /* compile an executable */
            if (empty_makefile_section(filename,"all") == 1) {
                sprintf(str, "gcc -Wall -std=%s -pedantic " \
                        "-O3 -o ${APP} %s/*.c -I%s %s",
                        c_standard, sourcedir, sourcedir, compile_args);
                add_makefile_entry_to_file(filename, "all", str);
            }
            if (empty_makefile_section(filename,"debug") == 1) {
                sprintf(str, "gcc -Wall -std=%s -pedantic " \
                        "-g -o ${APP} %s/*.c -I%s %s",
                        c_standard, sourcedir, sourcedir, compile_args);
                add_makefile_entry_to_file(filename, "debug", str);
            }
        }
        else {
            /* compile a shared library */
            if (empty_makefile_section(filename,"all") == 1) {
                sprintf(str, "gcc -shared -Wl,-soname,${SONAME} " \
                        "-std=%s -pedantic -fPIC " \
                        "-O3 -o ${LIBNAME} %s/*.c -I%s %s",
                        c_standard, sourcedir, sourcedir, compile_args);
                add_makefile_entry_to_file(filename, "all", str);
            }
            if (empty_makefile_section(filename,"debug") == 1) {
                sprintf(str, "gcc -shared -Wl,-soname,${SONAME} " \
                        "-std=%s -pedantic -fPIC " \
                        "-g -o ${LIBNAME} %s/*.c -I%s %s",
                        c_standard, sourcedir, sourcedir, compile_args);
                add_makefile_entry_to_file(filename, "debug", str);
            }
        }
    }
}

/* saves Java compile instructions to a makefile */
static void save_makefile_java(char * filename)
{
    char str[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char compile_args[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];
    char build_dir[BLOCK_SIZE];

    /* the project name */
    get_setting("project name", project_name);

    /* the type of project */
    get_setting("project type", project_type);

    /* compiler arguments */
    get_setting("compile", compile_args);

    /* get the directory within which the source code is located */
    get_setting("source dir", sourcedir);

    /* directory where results of compilation are held */
    get_setting("build dir", build_dir);

    if (strcmp(project_type,"java")==0) {
        if (is_library(project_name) == 0) {
            /* compile an executable */
            if (empty_makefile_section(filename,"all") == 1) {
                sprintf(str, "javac %s -d %s -sourcepath %s %s%c${APP}.java",
                        compile_args, build_dir, sourcedir,
                        sourcedir, DIRECTORY_SEPARATOR);
                add_makefile_entry_to_file(filename, "all", str);
            }
            if (empty_makefile_section(filename,"debug") == 1) {
                sprintf(str, "javac %s -d %s -sourcepath %s %s%c${APP}.java",
                        compile_args, build_dir, sourcedir,
                        sourcedir, DIRECTORY_SEPARATOR);
                add_makefile_entry_to_file(filename, "debug", str);
            }
        }
        else {
            /* TODO: compile a shared library */
        }
    }
}

/* saves a makefile */
void save_makefile(int no_of_binaries, char ** binaries)
{
    char directory[BLOCK_SIZE];
    char filename[BLOCK_SIZE];
    char str[BLOCK_SIZE];
    char svg_filename[BLOCK_SIZE];
    char project_type[BLOCK_SIZE];
    char project_version[BLOCK_SIZE];
    char project_name[BLOCK_SIZE];
    char compile_args[BLOCK_SIZE];
    char commandline[BLOCK_SIZE];
    char sourcedir[BLOCK_SIZE];

    /* get the project directory */
    get_setting("directory", directory);

    /* the type of project */
    get_setting("project type", project_type);

    /* the project name */
    get_setting("project name", project_name);

    /* the project version */
    get_setting("version", project_version);

    /* compiler arguments */
    get_setting("compile", compile_args);

    /* command line project */
    get_setting("commandline", commandline);

    /* get the directory within which the source code is located */
    get_setting("source dir", sourcedir);

    /* path and filename */
    sprintf(filename,"%s%cMakefile", directory,
            DIRECTORY_SEPARATOR);

    save_makefile_as(filename);

    /* add lines to the makefile if they don't exist */
    add_makefile_entry_to_file(filename, "source",
                               "tar -cvf ../${APP}_${VERSION}.orig.tar " \
                               "../${APP}-${VERSION} --exclude-vcs");
    add_makefile_entry_to_file(filename, "source",
                               "gzip -f9n ../${APP}_${VERSION}.orig.tar");
    add_makefile_entry_to_file(filename, "sourcedeb",
                               "tar -cvf ../${APP}_${VERSION}.orig.tar " \
                               "../${APP}-${VERSION} --exclude-vcs " \
                               "--exclude 'debian'");
    add_makefile_entry_to_file(filename, "sourcedeb",
                               "gzip -f9n ../${APP}_${VERSION}.orig.tar");

    save_makefile_install(filename, "install", no_of_binaries, binaries,
                          project_name, project_type, commandline,
                          directory, svg_filename);

    save_makefile_uninstall(filename, project_name,
                            commandline, no_of_binaries, binaries);

    if (is_library(project_name) != 0) {
        /* this install type will be used when creating packages,
           and excludes links */
        save_makefile_install(filename, "instlib", no_of_binaries, binaries,
                              project_name, project_type, commandline,
                              directory, svg_filename);
    }

    save_makefile_c(filename);
    save_makefile_cpp(filename);
    save_makefile_vala(filename);
    save_makefile_scala(filename);
    save_makefile_java(filename);

    if (is_library(project_name) == 0) {
        sprintf(str,"%s ${APP} \\#* \\.#* gnuplot* " \
            "*.png %s/*.substvars %s/*.log",
            COMMAND_DELETE, DEB_SUBDIR, DEB_SUBDIR);
    }
    else {
        sprintf(str,"%s ${LIBNAME} \\#* \\.#* gnuplot* " \
            "*.png %s/*.substvars %s/*.log",
            COMMAND_DELETE, DEB_SUBDIR, DEB_SUBDIR);
    }
    add_makefile_entry_to_file(filename, "clean", str);

    sprintf(str,"%sr deb.* %s/${APP} %s/${ARCH_TYPE}",
            COMMAND_DELETE, DEB_SUBDIR, RPM_SUBDIR);
    add_makefile_entry_to_file(filename, "clean", str);
    add_makefile_entry_to_file(filename, "clean",
                               "rm -f ../${APP}*.deb " \
                               "../${APP}*.changes " \
                               "../${APP}*.asc " \
                               "../${APP}*.dsc");
    sprintf(str,"%s %s/*.src.rpm %s/*.gz %s/*.xz",
            COMMAND_DELETE, RPM_SUBDIR, ARCH_SUBDIR, ARCH_SUBDIR);
    add_makefile_entry_to_file(filename, "clean", str);
    sprintf(str,"%s %s/*.gz %s/*.pet %s/*.txz",
            COMMAND_DELETE,
            PUPPY_SUBDIR, PUPPY_SUBDIR, SLACK_SUBDIR);
    add_makefile_entry_to_file(filename, "clean", str);

    if (is_qt_project() != 0) {
        /* remove QT build directory */
        sprintf(str, "%sr ${GUI_DIR}", COMMAND_DELETE);
        add_makefile_entry_to_file(filename, "clean", str);
    }

    replace_build_script_version(filename,
                                 project_name,
                                 project_version);
}
