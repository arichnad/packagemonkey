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

#include "settings.h"

/* returns the name of the settings directory */
static void settings_directory_name(char * directory)
{
	sprintf(directory,"%s%c.%s",getenv("HOME"),DIRECTORY_SEPARATOR,PROJECT_NAME);
}

/* returns the settings filename */
static void settings_filename(char * filename)
{
	char directory[BLOCK_SIZE];

	settings_directory_name(directory);
	sprintf(filename,"%s%csettings.dat", directory, DIRECTORY_SEPARATOR);
}

/* initialise the settings */
int init_settings()
{
	char directory[BLOCK_SIZE];
	char command[BLOCK_SIZE];
	int retval;

	settings_directory_name(directory);
	if (directory_exists(directory)!=0) return 0;

	sprintf(command,"%s %s", COMMAND_MKDIR, directory);
	retval = system(command);
	
	return retval;
}

/* returns the value of a setting */
int get_setting(char * setting_name, char * value)
{
	int index = -1, i, j, n, ctr;
	FILE * fp;
	char filename[BLOCK_SIZE],linestr[BLOCK_SIZE];

	value[0]=0;

	init_settings();
	settings_filename(filename);

	fp = fopen(filename,"r");
	if (!fp) return -1;
	ctr = 0;
	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;

			for (i = 0; i < strlen(linestr); i++) {
				if (linestr[i] == SETTINGS_SEPARATOR) {
					break;
				}
			}
			if (i < strlen(linestr)) {
				if (strncmp(linestr,setting_name,i)==0) {
					/* record the index within the file */
					index = ctr;
					/* extract the value */
					n = 0;
					for (j = i+1; j < strlen(linestr); j++) {
						if ((linestr[j] != 13) &&
							(linestr[j] != 10)) {
							value[n++] = linestr[j];
						}
					}
					value[n]=0;
					break;
				}
			}
			ctr++;
		}		
	}
	fclose(fp);
	return index;
}

/* adds or edits a setting */
int add_setting(char * setting_name, char * value)
{
	FILE * fp, * fp_temp;
	int ctr, retval, index=-1;
	char str[BLOCK_SIZE];
	char filename[BLOCK_SIZE], linestr[BLOCK_SIZE];
	char temp_filename[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];

	init_settings();
	settings_filename(filename);

	if (file_exists(filename)==0) {
		str[0]=0;
		index = get_setting(setting_name, str);
	}

	if (index == -1) {
		if (file_exists(filename)==0) {
			/* the first entry in the settings file */
			fp = fopen(filename,"w");
			if (fp) {
				fprintf(fp,"%s%c%s\n",setting_name,SETTINGS_SEPARATOR,value);
				fclose(fp);
			}
			else {
				printf("Unable to write to file %s\n",filename);
			}
			return 0;
		}
		/* append a new entry to the settings file */
		fp = fopen(filename,"a");
		if (fp) {
			fprintf(fp,"%s%c%s\n",setting_name,SETTINGS_SEPARATOR,value);
			fclose(fp);
		}
		else {
			printf("Unable to append to file %s\n",filename);
		}
		return 0;
	}

	/* edit an existing entry in the settings file */
	sprintf(temp_filename,"%s.new",filename);
	fp_temp = fopen(temp_filename,"w");
	fp = fopen(filename,"r");
	if ((!fp) || (!fp_temp)) return -1;
	ctr = 0;
	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
 
			if (ctr == index) {
				fprintf(fp_temp,"%s%c%s\n",setting_name,SETTINGS_SEPARATOR,value);
			}
			else {
				fprintf(fp_temp,"%s",linestr);
			}
			ctr++;
		}
	}
	fclose(fp);
	fclose(fp_temp);

	sprintf(commandstr, "%s %s %s", COMMAND_COPY, temp_filename, filename);
	retval = system(commandstr);
	sprintf(commandstr, "%s %s", COMMAND_DELETE, temp_filename);
	retval = system(commandstr);
	return retval;
}
