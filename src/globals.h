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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define VERSION "1.00"

#define PROJECT_NAME "packagemonkey"

/* maximum size of a string */
#define BLOCK_SIZE 512

/* character used to separate directories */
#define DIRECTORY_SEPARATOR '/'

/* location for temporary files */
#define TEMP_DIRECTORY "/tmp"

/* separator used within the settings */
#define SETTINGS_SEPARATOR '\t'

/* the maximum number of binary or library files given */
#define MAX_FILES 20

/* commands */
#define COMMAND_MKDIR  "mkdir"
#define COMMAND_COPY   "cp"
#define COMMAND_DELETE "rm -f"
#define COMMAND_MOVE   "mv"

/* subdirectories to create within the project directory */
#define ARCH_SUBDIR      "archpackage"
#define RPM_SUBDIR       "rpmpackage"
#define DEB_SUBDIR       "debian"
#define PUPPY_SUBDIR     "puppypackage"
#define EBUILD_SUBDIR    "ebuildpackage"
#define SLACK_SUBDIR     "slackpackage"
#define BUILDROOT_SUBDIR "buildroot"

/* character used to separate multiple lines within a string */
#define STRING_SEPARATOR '^'

/* default C/C++ standards */
#define DEFAULT_C_STANDARD   "gnu99"
#define DEFAULT_CPP_STANDARD "c++11"

void trim(char * str);
int is_numeric(char * str);
int file_exists(char * filename);
int directory_exists(char * directory);
void get_file_extension(char * filename, char * extension);
void get_subdirectory(char * directory, char * subdirectory);
int valid_project_name(char * project_name);
int match_string(char * str, char ** strings, int no_of_strings);
int valid_email(char * email_address);
int valid_gpg(char * email_address);
void string_to_lower(char * str, char * result);
void string_to_upper(char * str, char * result);
void detect_project_type(char * directory, char * project_type);
int valid_description(char * description);
int replace_build_script_version(char * filename,
                                 char * project_name,
                                 char * project_version);
int separate_files(char * files, char ** result, int max_files);
int is_library(char * project_name);
int file_is_library(char * filename);
int contains_char(char * str, char search);
int is_script(char * filename);
int files_exist(char * directory, char * file_extension);
void print_script(char * source_script, char * name);
void directory_size(char * directory,
                    char * dir_size);
void get_library_path(char * path);
void script_version_numbers(FILE * fp,
                            char * script_name);
void save_description(FILE * fp,
                      char * description,
                      int leading_space);
int is_script_language(char * project_type);
int is_c_language(char * project_type);
void relative_install_path(char * filename,
                           char * relative,
                           char * binaries,
                           char * project_name);

#endif
