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

/* returns the directories for a set of filenames */
int get_directories(char ** filenames, int no_of_filenames,
					char ** directories)
{
	int i, j, k, winner;
	int no_of_directories = 0;
	char str[BLOCK_SIZE];
	char * temp;

	for (i = 0; i < no_of_filenames; i++) {
		for (j = 0; j < strlen(filenames[i]); j++) {
			if (filenames[i][j] == DIRECTORY_SEPARATOR) {
				/* get the directory */
				for (k = 0; k < j; k++) {
					str[k] = filenames[i][k];
				}
				str[k] = 0;
				/* does the directory exist? */
				for (k = 0; k < no_of_directories; k++) {
					if (strcmp(directories[k],str)==0) break;
				}
				/* store the directory */
				if (k == no_of_directories) {
					directories[no_of_directories] =
						(char*)malloc(j+1);
					sprintf(directories[no_of_directories],
							"%s",str);
					no_of_directories++;
				}
			}
		}
	}

	/* sort in order of string length */
	for (i = 0; i < no_of_directories; i++) {
		winner = i;
		for (j = i+1; j < no_of_directories; j++) {
			if (strlen(directories[j]) <
				strlen(directories[winner])) {
				winner = j;
			}
		}
		if (winner != i) {
			temp = directories[i];
			directories[i] = directories[winner];
			directories[winner] = temp;
		}
	}

	return no_of_directories;
}

/* returns the given directory with the root directory removed */
char * get_subdirectory_string(char * directory)
{
	int i;

	for (i = 0; i < strlen(directory)-1; i++) {
		if (directory[i] == DIRECTORY_SEPARATOR) {
			return &directory[i+1];
		}
	}
	return 0;
}
