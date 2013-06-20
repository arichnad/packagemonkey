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

#include "directory.h"

/* This is a modified version of the recursive directory
   reading function from:
   http://rosettacode.org/wiki/Walk_Directory_Tree#Library:_POSIX
*/
int walk_recur(char *dname, regex_t *reg, int spec,
			   char ** filenames, int * no_of_filenames,
			   int max_filenames)
{
    struct dirent *dent;
    DIR *dir;
    struct stat st;
    char fn[FILENAME_MAX];
    int res = WALK_OK;
    int len = strlen(dname);
    if (len >= FILENAME_MAX - 1)
        return WALK_NAMETOOLONG;

    strcpy(fn, dname);
    fn[len++] = '/';

    if (!(dir = opendir(dname))) {
        return WALK_BADIO;
    }

    errno = 0;
    while ((dent = readdir(dir))) {
        if (!(spec & WS_DOTFILES) && dent->d_name[0] == '.')
            continue;
        if (!strcmp(dent->d_name, ".") ||
			!strcmp(dent->d_name, ".."))
            continue;

        strncpy(fn + len, dent->d_name, FILENAME_MAX - len);
        if (lstat(fn, &st) == -1) {
            warn("Can't stat %s", fn);
            res = WALK_BADIO;
            continue;
        }

        /* don't follow symlink unless told so */
        if (S_ISLNK(st.st_mode) && !(spec & WS_FOLLOWLINK))
            continue;

        /* will be false for symlinked dirs */
        if (S_ISDIR(st.st_mode)) {
            /* recursively follow dirs */
            if ((spec & WS_RECURSIVE))
                walk_recur(fn, reg, spec, filenames,
						   no_of_filenames, max_filenames);

            if (!(spec & WS_MATCHDIRS)) continue;
        }
        else {
            /* pattern match */
			if (*no_of_filenames < max_filenames) {
				filenames[*no_of_filenames] =
					(char*)malloc(strlen(fn) + 1);
				sprintf(filenames[*no_of_filenames], "%s", fn);
				*no_of_filenames = *no_of_filenames + 1;
			}
        }
    }

    if (dir) closedir(dir);
    return res ? res : errno ? WALK_BADIO : WALK_OK;
}

/* returns the files contained within a given directory */
int files_in_directory(char *dname, char *pattern, int spec,
					   char ** filenames, int * no_of_filenames,
					   int max_filenames)
{
    regex_t r;
    int res;

	*no_of_filenames = 0;

    if (regcomp(&r, pattern, REG_EXTENDED | REG_NOSUB))
        return WALK_BADPATTERN;
    res = walk_recur(dname, &r, spec,
					 filenames, no_of_filenames,
					 max_filenames);
    regfree(&r);

    return res;
}
