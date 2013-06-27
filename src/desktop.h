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

#ifndef DESKTOP_H
#define DESKTOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <zlib.h>
#include "globals.h"
#include "settings.h"
#include "pnglite.h"

int save_desktop();
int valid_main_category(char * category);
int valid_additional_category(char * category);
void parse_desktop_category(char * str, char * main, char * additional);
void show_categories();
void show_categories_main();
void show_categories_additional();
void free_desktop_to_puppy_desktop(char * free_desktop_categories,
								   char * puppy_desktop_categories);

#endif
