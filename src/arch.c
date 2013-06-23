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

#include "arch.h"

/* save the PKGBUILD file */
static void save_PKGBUILD(char * directory)
{
	char email_address[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char release[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char homepage[BLOCK_SIZE];
	char license[BLOCK_SIZE];
	char depends[BLOCK_SIZE];
	char build_depends[BLOCK_SIZE];
	char source_package[BLOCK_SIZE];
	char filename[BLOCK_SIZE];
	FILE * fp;

	sprintf(filename, "%s%cPKGBUILD",
			directory, DIRECTORY_SEPARATOR);

	fp = fopen(filename,"w");
	if (!fp) return;

	get_setting("email", email_address);
	get_setting("project name", project_name);
	get_setting("version", version);
	get_setting("release", release);
	get_setting("description", description);
	get_setting("homepage",homepage);
	get_setting("license",license);
	get_setting("depends arch",depends);
	get_setting("build arch",build_depends);
	get_setting("source package",source_package);

    fprintf(fp, "# Maintainer: %s\n", email_address);
    fprintf(fp, "pkgname=%s\n",project_name);
    fprintf(fp, "pkgver=%s\n", version);
	fprintf(fp, "pkgrel=%s\n", release);
	fprintf(fp, "pkgdesc=\"%s\"\n", description);
	fprintf(fp, "%s", "arch=('any')\n");
	fprintf(fp, "url=\"%s\"\n",homepage);
	fprintf(fp, "license=('%s')\n",license);
	fprintf(fp, "groups=()\n");
	fprintf(fp, "depends=(%s)\n",depends);
	fprintf(fp, "makedepends=('base-devel' %s)\n",build_depends);
	fprintf(fp, "optdepends=()\n");
	fprintf(fp, "provides=()\n");
	fprintf(fp, "conflicts=()\n");
	fprintf(fp, "replaces=()\n");
	fprintf(fp, "backup=()\n");
	fprintf(fp, "options=()\n");
	fprintf(fp, "install=\n");
	fprintf(fp, "changelog=\n");
	if (strlen(source_package) == 0) {
	    fprintf(fp, "%s", "source=($pkgname_$pkgver.orig.tar.gz)\n");
    }
	else {
	    fprintf(fp, "source=(%s)\n",source_package);
    }
	fprintf(fp, "noextract=()\n");
	fprintf(fp, "md5sums=() #generate with 'makepkg -g'\n");
	fprintf(fp, "build() {\n");
	fprintf(fp, "  cd \"$srcdir/$pkgname-$pkgver\"\n");
	fprintf(fp, "  ./configure --prefix=/usr\n");
	fprintf(fp, "  make\n");
	fprintf(fp, "}\n");
	fprintf(fp, "package() {\n");
	fprintf(fp, "  cd \"$srcdir/$pkgname-$pkgver\"\n");
	fprintf(fp, "  make DESTDIR=\"$pkgdir/\" install\n");
	fprintf(fp, "}\n");

    fclose(fp);
}

int save_arch()
{
	char directory[BLOCK_SIZE];
	char arch_directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int retval=0;

	get_setting("directory",directory);
	sprintf(arch_directory,"%s%carchpackage",
			directory, DIRECTORY_SEPARATOR);

	/* create a directory for building arch packages */
	if (directory_exists(arch_directory)==0) {
		sprintf(commandstr,"%s %s",
				COMMAND_MKDIR, arch_directory);
		retval = system(commandstr);
	}

	save_PKGBUILD(arch_directory);

	return retval;
}
