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

#ifndef MAKEFILE_H
#define MAKEFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "globals.h"
#include "settings.h"
#include "directory.h"

void save_makefile_as(char * filename);
void save_makefile(int no_of_binaries, char ** binaries);
int get_makefile_entry_from_file(char * makefilename, char * section, char * entry);
int get_makefile_entry(char * section, char * entry);
int add_makefile_entry_to_file(char * makefilename, char * section, char * entry);
int add_makefile_entry(char * section, char * entry);
int empty_makefile_section(char * makefilename,
                           char * section);
int save_configure(char * directory);

#endif
