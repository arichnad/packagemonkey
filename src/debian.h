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

#ifndef DEBIAN_H
#define DEBIAN_H

#define DEFAULT_DEBIAN_VERSION 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "globals.h"
#include "settings.h"
#include "directory.h"

void debian_list_sections();
int debian_valid_section(char * section);
int save_debian(int no_of_binaries, char ** binaries);
int debian_changelog_to_spec(char * directory);
void debian_parse_changelog_header(char * linestr,
								   char * version);
void debian_parse_changelog_footer(char * linestr,
								   char * email_address,
								   char * dayname,
								   char * month,
								   char * monthday,
								   char * year);

#endif
