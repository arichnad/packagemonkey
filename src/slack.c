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

#include "slack.h"

static void save_slack_desc(char * directory)
{
	char filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char description[BLOCK_SIZE];
	char str[BLOCK_SIZE];
	FILE * fp;
	int i, max, n, ctr = 0, start;

	get_setting("project name", project_name);
	get_setting("description brief", description_brief);
	get_setting("description", description);

	sprintf(filename, "%s%c%s%cslack-desc",
			directory, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR, DIRECTORY_SEPARATOR);

	fp = fopen(filename, "w");
	if (!fp) return;

	fprintf(fp, "%s", "# HOW TO EDIT THIS FILE:\n");
	fprintf(fp, "%s", "# The \"handy ruler\" below makes it easier to " \
			"edit a package description.  Line\n");
	fprintf(fp, "%s", "# up the first '|' above the ':' following the " \
			"base package name, and the '|' on\n");
	fprintf(fp, "%s", "# the right side marks the last column you can " \
			"put a character in.  You must make\n");
	fprintf(fp, "%s", "# exactly 11 lines for the formatting to be " \
			"correct.  It's also customary to\n");
	fprintf(fp, "%s", "# leave one space after the ':'.\n\n");

	for (i = 0; i < strlen(project_name); i++) {
		fprintf(fp, "%s", " ");
	}
	fprintf(fp, "%s", "|-----handy-ruler");
	for (i = 0; i < 79 - strlen(project_name) - 17; i++) {
		fprintf(fp, "%s", "-");
	}
	fprintf(fp, "%s", "|\n");

	sprintf(str, "%s: %s (%s)\n",
			project_name, project_name, description_brief);

	if (strlen(str) >= 79) {
		str[78] = '\n';
		str[79] = 0;
		printf("WARNING: brief description exceeds 79 character line " \
			   "length within slack-desc\n");
	}
	fprintf(fp, "%s", str);
	
	fprintf(fp, "%s:\n", project_name);
	max = 79 - strlen(project_name) - 2;
	for (i = 0; i < 9; i++) {
		fprintf(fp, "%s:", project_name);
		n = 0;
		start = ctr;

		if (ctr < strlen(description)) {
			fprintf(fp, "%s", " ");
		}

		while ((n < max) &&
			   (ctr < strlen(description))) {			
			ctr++;
			n++;
		}

		if (n == max) {
			while ((ctr > 1) && (description[ctr] != ' ')) {
				ctr--;
				n--;
			}
			while ((ctr > 1) && (description[ctr] == ' ')) {
				ctr--;
				n--;
			}
			ctr++;
			n++;
			strncpy(str, &description[start], n);
			str[n] = 0;
			fprintf(fp, "%s", str);
			start += n;
			ctr++;
			n++;
		}
		else {
			if (start < strlen(description)) {
				fprintf(fp, "%s", &description[start]);
				start = strlen(description);
			}
		}
		
		fprintf(fp, "%s", "\n");
		
	}

	fclose(fp);
}

int save_slack()
{
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char subdir[BLOCK_SIZE];
	int retval = 0;

	get_setting("directory", directory);

	/* create the slackpackage subdirectory */
	sprintf(subdir, "%s%c%s",
			directory, DIRECTORY_SEPARATOR,
			SLACK_SUBDIR);
	if (directory_exists(subdir) == 0) {
		sprintf(commandstr, "%s %s",
				COMMAND_MKDIR, subdir);
		retval = system(commandstr);
	}

	/* save the package description */
	save_slack_desc(directory);

	return retval;
}
