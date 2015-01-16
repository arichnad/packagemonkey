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

#include "help.h"

void show_help()
{
    printf("%s v%s - because packaging is harder than " \
           "it needs to be\n\n", PROJECT_NAME, VERSION);

    printf(" -n  --name <project name>         " \
           "The full name or title of the project\n");
    printf(" -d  --dir <directory>             " \
           "Directory of the project to be packaged\n");
    printf(" -c  --cmd                         " \
           "Used to indicate this is a command line project\n");
    printf(" -b  --binaries <files>            " \
           "Directory containing binaries to be packaged\n");
    printf("     --compile <args>              " \
           "Additional arguments to add to the compilation\n");
    printf(" -l  --license <type>              " \
           "The license type (gpl2|gpl3|lgpl2|lgpl3|agpl|\n" \
           "                                   " \
           "agpl3|mit|bsd|apache|mozilla)\n");
    printf(" -e  --email <address>             " \
           "Set full name and email address\n");
    printf("     --brief <description>         " \
           "A brief one line description of the project\n");
    printf("     --desc <description>          " \
           "A full description of the project\n");
    printf("     --homepage <url>              " \
           "URL of the project home page\n");
    printf("     --src <url>                   " \
           "URL of the compressed source code\n");
    printf("     --srcdir <directory>          " \
           "Subdirectory within which source code will be\n" \
           "                                   " \
           "located (typically \"src\")\n");
    printf("     --section <sections>          " \
           "Debian sections\n");
    printf("     --categories <cat>            " \
           ".desktop file categories\n");
    printf(" -m  --mime <types>                " \
           "Mime types which the project can handle\n");
    printf("     --dependsdeb <packages>       " \
           "Deb packages which the project depends upon\n");
    printf("     --dependsrpm <packages>       " \
           "RPM packages which the project depends upon\n");
    printf("     --dependsrpmdistro <packages> " \
           "Distro specific dependencies\n");
    printf("     --dependsarch <packages>      " \
           "Arch packages which the project depends upon\n");
    printf("     --dependspuppy <packages>     " \
           "Puppy packages which the project depends upon\n");
    printf("     --dependsebuild <packages>    " \
           "Ebuild packages which the project depends upon\n");
    printf("     --builddeb <packages>         " \
           "Deb packages required to build the project\n");
    printf("     --buildrpm <packages>         " \
           "RPM packages required to build the project\n");
    printf("     --buildrpmdistro <packages>   " \
           "Distro specific build dependencies\n");
    printf("     --buildarch <packages>        " \
           "Arch packages required to build the project\n");
    printf("     --buildebuild <packages>      " \
           "Ebuild packages required to build the project\n");
    printf("     --list-sections               " \
           "List the possible Debian sections\n");
    printf("     --list-desktop                " \
           "List the possible desktop file categories\n");
    printf("     --debian <version>            " \
           "Set the Debian version\n");
    printf("     --mainscript <filename>       " \
           "If the project uses a scripted language then\n" \
           "                                   " \
           "this specifies the initial script to be run\n");
    printf("     --cstandard <standard>        " \
           "The standard to be used by gcc/g++ (c99, gnu99, etc)\n");
    printf("     --librarypath <paths>         " \
           "Specify LD_LIBRARY_PATH\n");
    printf("     --suggestsdeb                 " \
           "Suggested Debian packages\n");
    printf("     --suggestsarch                " \
           "Suggested Arch packages\n");
    printf(" -t  --tests                       " \
           "Run unit tests\n");
    printf("     --pythonversion               " \
           "Version of Python to use (2/3)\n");
    printf("     --pythonreq                   " \
           "Ebuild PYTHON_REQ_USE dependencies\n");
    printf(" -v  --version                     " \
           "Show or set the project version number\n");
    printf(" -r  --release                     " \
           "Set the project release number\n");
    printf("     --brselect                    " \
           "buildroot select entries from Config.in\n");
    printf("     --brdepends                   " \
           "buildroot depends entries from Config.in\n");
    printf("     --downloadsite                " \
           "buildroot URL to download source code from\n");
    printf("     --brdependencies              " \
           "buildroot makefile dependencies\n");
    printf("     --commit                      " \
           "Commit hash if downloading from a repo\n");
    printf(" -h  --help                        " \
           "Show help\n");
}
