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
	char * package_list[MAX_FILES];
	int i, no_of_packages;

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
	fprintf(fp, "%s", "arch=('i686' 'x86_64')\n");
	fprintf(fp, "url=\"%s\"\n",homepage);
	fprintf(fp, "license=('%s')\n",license);
	fprintf(fp, "%s","groups=()\n");

	fprintf(fp, "%s", "depends=(");
	no_of_packages =
		separate_files(depends, package_list,
					   MAX_FILES);
	for (i = 0; i < no_of_packages; i++) {
		fprintf(fp, " '%s'", package_list[i]);
		free(package_list[i]);
	}
	fprintf(fp,"%s",")\n");

	fprintf(fp, "%s", "makedepends=(");
	no_of_packages =
		separate_files(build_depends, package_list,
					   MAX_FILES);
	for (i = 0; i < no_of_packages; i++) {
		fprintf(fp, " '%s'", package_list[i]);
		if (i < no_of_packages-1) {
			fprintf(fp, "%s"," ");
		}
		free(package_list[i]);
	}
	fprintf(fp,"%s",")\n");

	fprintf(fp, "%s", "optdepends=()\n");
	fprintf(fp, "%s", "provides=()\n");
	fprintf(fp, "%s", "conflicts=()\n");
	fprintf(fp, "%s", "replaces=()\n");
	fprintf(fp, "%s", "backup=()\n");
	fprintf(fp, "%s", "options=()\n");
	fprintf(fp, "%s", "install=\n");
	fprintf(fp, "%s", "changelog=\n");
	if (strlen(source_package) == 0) {
	    fprintf(fp, "%s", "source=($pkgname-$pkgver.tar.gz)\n");
    }
	else {
	    fprintf(fp, "source=(%s)\n",source_package);
    }
	fprintf(fp, "%s", "noextract=()\n");
	fprintf(fp, "%s", "md5sums=()\n");
	fprintf(fp, "%s", "build() {\n");
	fprintf(fp, "%s", "  cd \"$srcdir/$pkgname-$pkgver\"\n");
	fprintf(fp, "%s", "  ./configure --prefix=/usr\n");
	fprintf(fp, "%s", "  make\n");
	fprintf(fp, "%s", "}\n");
	fprintf(fp, "%s", "package() {\n");
	fprintf(fp, "%s", "  cd \"$srcdir/$pkgname-$pkgver\"\n");
	if (is_library(project_name) == 0) {	
		fprintf(fp, "%s", "  make DESTDIR=\"$pkgdir/\" install\n");
	}
	else {
		fprintf(fp, "%s", "  make DESTDIR=\"$pkgdir/\" instlib\n");
	}
	fprintf(fp, "%s", "}\n");

	if (is_library(project_name) != 0) {
		fprintf(fp, "%s", "post_install() {\n");
		fprintf(fp, "%s", "  ln -sf /usr/lib/$pkgname-$pkgver.so.0.0.$pkgrel " \
				"/usr/lib/$pkgname.so.0\n");
		fprintf(fp, "%s", "  ln -sf /usr/lib/$pkgname-$pkgver.so.0.0.$pkgrel " \
				"/usr/lib/$pkgname.so\n");
		fprintf(fp, "%s", "  ldconfig\n");
		fprintf(fp, "%s", "}\n");
		fprintf(fp, "%s", "post_remove() {\n");
		fprintf(fp, "%s", "  ldconfig\n");
		fprintf(fp, "%s", "}\n");
	}

    fclose(fp);
}

static int save_script(char * directory)
{
	FILE * fp;
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char version[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char release[BLOCK_SIZE];

	get_setting("project name", project_name);
	get_setting("version", version);
	get_setting("release",release);

	sprintf(filename,"%s%carch.sh",
			directory, DIRECTORY_SEPARATOR);

	fp = fopen(filename,"w");
	if (!fp) return -1;

	fprintf(fp, "%s", "#!/bin/bash\n\n");

	fprintf(fp, "APP=%s\n",project_name);
	fprintf(fp, "PREV_VERSION=%s\n", version);
	fprintf(fp, "VERSION=%s\n",version);
	fprintf(fp, "RELEASE=%s\n",release);
	fprintf(fp, "%s", "ARCH_TYPE=`uname -m`\n");
	fprintf(fp, "%s", "CURRDIR=`pwd`\n");
	fprintf(fp, "SOURCE=%s/${APP}-${VERSION}.tar.gz\n",
			ARCH_SUBDIR);

	/* alter the version numbers */
	script_version_numbers(fp,"arch");

	/*fprintf(fp, "%s", "\n# Set the type of architecture\n");
	fprintf(fp, "sed -i \"s/arch=('any')/" \
			"arch=('${ARCH_TYPE}')/g\" \"%s/PKGBUILD\"\n",
			ARCH_SUBDIR);*/

	fprintf(fp, "%s", "\n# Create the source code\n");
	fprintf(fp, "%s", "make clean\n");
	fprintf(fp, "%s %s/*.gz\n", COMMAND_DELETE, ARCH_SUBDIR);
	fprintf(fp, "%s", "# having the root directory called name-version seems essential\n");
	fprintf(fp, "%s", "mv ../${APP} ../${APP}-${VERSION}\n");
	fprintf(fp, "%s", "tar -cvzf ${SOURCE} ../${APP}-${VERSION} --exclude-vcs\n");
	fprintf(fp, "%s", "# rename the root directory without the version number\n");
	fprintf(fp, "%s", "mv ../${APP}-${VERSION} ../${APP}\n");

	fprintf(fp, "%s", "\n# calculate the MD5 checksum\n");
	fprintf(fp, "%s", "CHECKSM=$(md5sum ${SOURCE})\n");
	fprintf(fp, "sed -i \"s/md5sums[^)]*)/" \
			"md5sums=(${CHECKSM%%%% *})/g\" %s/PKGBUILD\n",
			ARCH_SUBDIR);

	fprintf(fp, "\ncd %s\n", ARCH_SUBDIR);

	fprintf(fp, "%s", "\n# Create the package\n");
	fprintf(fp, "%s", "makepkg\n");

	fprintf(fp, "%s", "\n# Move back to the original directory\n");
	fprintf(fp, "%s", "cd ${CURRDIR}\n\n");

	fclose(fp);

	sprintf(commandstr,"chmod +x %s", filename);
	return system(commandstr);
}

int save_arch()
{
	char directory[BLOCK_SIZE];
	char arch_directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	int retval=0;

	get_setting("directory",directory);
	sprintf(arch_directory,"%s%c%s",
			directory, DIRECTORY_SEPARATOR,
			ARCH_SUBDIR);

	/* create a directory for building arch packages */
	if (directory_exists(arch_directory)==0) {
		sprintf(commandstr,"%s %s",
				COMMAND_MKDIR, arch_directory);
		retval = system(commandstr);
	}

	save_PKGBUILD(arch_directory);
	save_script(directory);

	return retval;
}
