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

#include "help.h"

void show_help()
{
	printf("%s v%s - because packaging is harder than " \
		   "it needs to be\n\n", PROJECT_NAME, VERSION);

	printf(" -d  --dir <directory>         " \
		   "Set the working directory\n");
	printf(" -s  --scripts <directory>     " \
		   "Set directory containing scripts to be packaged\n");
	printf(" -b  --binaries <files>        " \
		   "Directory containing binaries to be packaged\n");
	printf("     --compile <args>          " \
		   "Additional arguments to add to the compilation\n");
	printf(" -l  --license <type>          " \
		   "The license type (gpl2|gpl3|lgpl2|lgpl3|mit|" \
		   "bsd|apache|mozilla)\n");
	printf(" -e  --email <address>         " \
		   "Set full name and email address\n");
	printf("     --brief <description>     " \
		   "A brief one line description of the project\n");
	printf("     --desc <description>      " \
		   "A full description of the project\n");
	printf("     --homepage <url>          " \
		   "URL of the project home page\n");
	printf("     --section <sections>      " \
		   "Debian sections\n");
	printf("     --categories <cat>        " \
		   ".desktop file categories\n");
	printf("     --dependsdeb <packages>   " \
		   "Deb packages which the project depends upon\n");
	printf("     --builddeb <packages>     " \
		   "Deb packages required to build the project\n");
	printf("     --dependsrpm <packages>   " \
		   "RPM packages which the project depends upon\n");
	printf("     --buildrpm <packages>     " \
		   "RPM packages required to build the project\n");
	printf("     --list-sections           " \
		   "List the possible Debian sections\n");
	printf("     --list-desktop            " \
		   "List the possible desktop file categories\n");
	printf("     --debian <version>        " \
		   "Set the Debian version\n");
	printf(" -t  --tests                   " \
		   "Run unit tests\n");
	printf(" -v  --version                 " \
		   "Show or set the project version number\n");
	printf(" -r  --release                 " \
		   "Set the project release number\n");
	printf(" -h  --help                    " \
		   "Show help\n");
}
