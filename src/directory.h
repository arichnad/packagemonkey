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

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <err.h>
#include "globals.h"
#include "settings.h"

enum {
    WALK_OK = 0,
    WALK_BADPATTERN,
    WALK_NAMETOOLONG,
    WALK_BADIO
};

#define WS_NONE		0
#define WS_RECURSIVE	(1 << 0)
#define WS_DEFAULT	WS_RECURSIVE
#define WS_FOLLOWLINK	(1 << 1)	/* follow symlinks */
#define WS_DOTFILES	(1 << 2)	/* per unix convention, .file is hidden */
#define WS_MATCHDIRS	(1 << 3)	/* if pattern is used on dir names too */

int files_in_directory(char *dname, char *pattern, int spec,
					   char ** filenames, int * no_of_filenames,
					   int max_filenames);
int get_directories(char ** filenames, int no_of_filenames,
					char ** directories);
char * get_subdirectory_string(char * directory);

#endif
