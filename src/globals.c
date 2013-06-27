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

#include "globals.h"

/* returns non-zero value if the given file exists */
int file_exists(char * filename)
{
	FILE * fp = fopen(filename,"r");
	if (fp) {
		fclose(fp);
		return 1;
	}
	return 0;
}

int directory_exists(char * name)
{
	struct stat sts;
	if ((stat(name, &sts)) == -1) {
		return 0;
	}
	return 1;
}

/* returns teh extension of a filename */
void get_file_extension(char * filename, char * extension)
{
	int i, j, ctr=0;

	extension[0]=0;

	for (i = strlen(filename)-1; i >= 0; i--) {
		if (filename[i]=='.') break;
	}
	if (i > 0) {
		for (j = i+1; j < strlen(filename); j++) {
			extension[ctr++] = filename[j];
		}
		extension[ctr]=0;
	}
}

/* returns the last directory in the given tree */
void get_subdirectory(char * directory, char * subdirectory)
{
	int i, j, ctr=0;

	for (i = strlen(directory)-2; i >= 0; i--) {
		if (directory[i] == DIRECTORY_SEPARATOR) {
			break;
		}
	}

	if (i > 0) {
		for (j = i+1; j < strlen(directory); j++) {
			if (directory[j] != DIRECTORY_SEPARATOR) {
				subdirectory[ctr++] = directory[j];
			}
		}
	}
	else {
		for (j = 0; j < strlen(directory); j++) {
			if (directory[j] != DIRECTORY_SEPARATOR) {
				subdirectory[ctr++] = directory[j];
			}
		}
	}
	subdirectory[ctr] = 0;
}

/* returns non-zero value if the given string is a valid project name */
int valid_project_name(char * project_name)
{
	int i;

	for (i = 0; i < strlen(project_name); i++) {
		if (((project_name[i] < 'a') ||
			 (project_name[i] > 'z')) &&
			((project_name[i] < '0') ||
			 (project_name[i] > '9'))) {
			return 0;
		}
	}
	return 1;
}

/* returns a non-zero value if the given string is within the set of strings */
int match_string(char * str, char ** strings, int no_of_strings)
{
	int i;

	for (i = 0; i < no_of_strings; i++) {
		if (strcmp(str,strings[i])==0) {
			return 1;
		}
	}
	return 0;
}

/* converts a string to lower case */
void string_to_lower(char * str, char * result)
{
	int i;

	/* convert to lower case */
	for (i = 0; i < strlen(str); i++) {
		result[i] = tolower(str[i]);
	}
	result[i] = 0;
}

/* converts a string to upper case */
void string_to_upper(char * str, char * result)
{
	int i;

	/* convert to lower case */
	for (i = 0; i < strlen(str); i++) {
		result[i] = toupper(str[i]);
	}
	result[i] = 0;
}

/* returns a non-zero value if the given email addess is in the correct format */
int valid_email(char * email_address)
{
	int i,state=0;

	for (i = 0; i < strlen(email_address); i++) {
		switch(state) {
		case 0: {
			if ((email_address[i]>='a') &&
				(email_address[i]<='z')) {
				state++;
			}
			break;
		}	
		case 1: {
			if (email_address[i] == ' ') {
				state++;
			}
			break;
		}
		case 2: {
			if (email_address[i] == '<') {
				state++;
			}
			break;
		}
		case 3: {
			if ((email_address[i]>='a') &&
				(email_address[i]<='z')) {
				state++;
			}
			break;
		}	
		case 4: {
			if (email_address[i] == '@') {
				state++;
			}
			break;
		}
		case 5: {
			if ((email_address[i]>='a') &&
				(email_address[i]<='z')) {
				state++;
			}
			break;
		}	
		case 6: {
			if (email_address[i] == '.') {
				state++;
			}
			break;
		}
		case 7: {
			if ((email_address[i]>='a') &&
				(email_address[i]<='z')) {
				state++;
			}
			break;
		}	
		case 8: {
			if (email_address[i] == '>') {
				state++;
			}
			break;
		}
		}
	}
	if (state == 9) return 1;
	return 0;
}

/* returns a non-zero value if the given email address exists as a GPG key */
int valid_gpg(char * email_address)
{
	FILE * fp;
	int found = 0;
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char result_filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE];

	sprintf(directory, "%s%c.%s",
			getenv("HOME"), DIRECTORY_SEPARATOR, PROJECT_NAME);

	sprintf(result_filename, "%s/test.txt",	directory);

	sprintf(commandstr, "gpg --list-keys | grep \"%s\" > %s",
			email_address, result_filename);
	found = system(commandstr);

	/* check that the email address exists within the GPG keys list */
	fp = fopen(result_filename,"r");
	if (!fp) return 0;

	found = 0;
	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			if (strstr(linestr, email_address)!=NULL) {
				found = 1;
				break;
			}
		}		
	}

	fclose(fp);

	if (found == 0) return 0;

	/* delete the temporary file */
	sprintf(commandstr,"%s %s",COMMAND_DELETE,result_filename);
	found = system(commandstr);

	return 1;
}

/* attempts to detect the type of project */
void detect_project_type(char * directory, char * project_type)
{
	char commandstr[BLOCK_SIZE];
	char temp_filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE];
	int i, j, ctr=0;
	FILE * fp;
	const int no_of_types = 7;
	const char * proj_type[] = {
		"c","cpp","py","rbbas","rbfrm",
		"vala","java"
	};

	sprintf(temp_filename,"%s%cpm_temp_result",
			TEMP_DIRECTORY, DIRECTORY_SEPARATOR);

	project_type[0] = 0;

	/* for each type of file extension */
	for (ctr = 0; ctr < 2; ctr++) {
		for (i = 0; i < no_of_types; i++) {

			if (ctr == 0) {
				sprintf(commandstr,"ls %s%c*.%s > %s 2>&1",
						directory, DIRECTORY_SEPARATOR, proj_type[i],
						temp_filename);
			}
			else {
				sprintf(commandstr,"ls %s%csrc%c*.%s > %s 2>&1",
						directory, DIRECTORY_SEPARATOR,
						DIRECTORY_SEPARATOR, proj_type[i],
						temp_filename);
			}

			j = system(commandstr);

			if (file_exists(temp_filename) == 0) continue;
			fp = fopen(temp_filename,"r");
			if (!fp) continue;
		
			while (!feof(fp)) {
				if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
					if (strlen(linestr) == 0) continue;
					for (j = 0; j < strlen(linestr); j++) {
						if (linestr[j] == ':') {
							break;
						}
					}
					if (j == strlen(linestr)) {
						sprintf(project_type,"%s",proj_type[i]);
						break;
					}
				}
			}

			fclose(fp);
			if (strlen(project_type) > 0) break;
		}
		if (strlen(project_type) > 0) break;
	}
}

/* returns 1 if the given description is valid */
int valid_description(char * description)
{
	int i;
	const int no_of_articles = 4;
	const char * article[] = {
		"A","An","This","The"
	};
	char str[BLOCK_SIZE];

	/* non-zero length */
	if (strlen(description) == 0) return -1;

	/* first character should be upper case */
	description[0] = toupper(description[0]);

	/* should not begin with an indefinite article */
	for (i = 0; i < no_of_articles; i++) {
		if (strlen(description) > strlen(article[i])+1) {
			sprintf(str,"%s ",article[i]);
			if (strncmp(description, str, strlen(str))==0) return -2;
		}
	}

	/* should not begin with a space or tab */
	if ((description[0] == ' ') ||
		(description[0] == '\t')) {
		return -3;
	}

	return 1;
}

/* changes the version number and project name within the build script */
int replace_build_script_version(char * filename,
								 char * project_name,
								 char * project_version)
{
	char linestr[BLOCK_SIZE];
	char new_filename[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	FILE * fp, * fp_new;
	int retval=0;

	sprintf(new_filename,"%s.new", filename);

	fp_new = fopen(new_filename,"w");
	if (!fp_new) return -1;
	
	fp = fopen(filename,"r");
	if (!fp) return -1;

	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			if (strncmp(linestr,"VERSION=",8)==0) {
				fprintf(fp_new,"VERSION=%s\n",project_version);
			}
			else {
				if (strncmp(linestr,"APP=",4)==0) {
					fprintf(fp_new,"APP=%s\n",project_name);
				}
				else {
					fprintf(fp_new,"%s",linestr);
				}
			}
		}
	}

	fclose(fp);
	fclose(fp_new);

	sprintf(commandstr,"%s %s %s",COMMAND_COPY,
			new_filename,filename);
	retval = system(commandstr);
	sprintf(commandstr,"%s %s",COMMAND_DELETE,new_filename);
	retval = system(commandstr);
	return retval;
}

/* separates the given files string into an array of separate filenames */
int separate_files(char * files, char ** result, int max_files)
{
	int i,ctr=0,initial=1;
	char filename[BLOCK_SIZE];
	int no_of_files = 0;

	for (i = 0; i < strlen(files); i++) {
		if ((files[i] == ',') ||
			(files[i] == ';') ||
			(files[i] == '|') ||
			(i == strlen(files)-1)) {
			/* end of filename */
			if ((i == strlen(files)-1) &&
				(files[i] != ',') &&
				(files[i] != ';') &&
				(files[i] != '|') &&
				(files[i] != 10) &&
				(files[i] != 13)) {
				filename[ctr++] = files[i];
			}
			filename[ctr]=0;
			result[no_of_files] = (char*)malloc(sizeof(filename)+1);
			sprintf(result[no_of_files],"%s",filename);
			no_of_files++;
			if (no_of_files >= max_files) {
				break;
			}
			/* reset */
			initial = 1;
			ctr = 0;
		}
		else {
			/* looking for the beginning of a filename */
			if (initial == 1) {
				if ((files[i] != ' ') &&
					(files[i] != ',') &&
					(files[i] != ';') &&
					(files[i] != '|') &&
					(files[i] != '\t')) {
					initial = 0;
				}
			}
			/* storing a filename */
			if (initial == 0) {
				if ((files[i] != 10) &&
					(files[i] != 13)) {
					filename[ctr++] = files[i];
				}
			}
		}
	}

	if ((ctr > 0) && (no_of_files < max_files-1)) {
		filename[ctr]=0;

		sprintf(result[no_of_files],"%s",filename);
		no_of_files++;
	}
	return no_of_files;
}

/* returns a non-zero value if the given project name is a library.
   According to the Debian convention all library names
   should begin with "lib" */
int is_library(char * project_name)
{
	if (strlen(project_name) < 3) {
		return 0;
	}
	if (strncmp(project_name,"lib",3)==0) {
		return 1;
	}
	return 0;
}

/* is the given file a library? */
int file_is_library(char * filename)
{
	if (strlen(filename) < 4) return 0;
	if ((filename[strlen(filename)-3] == '.') &&
		(filename[strlen(filename)-2] == 's') &&
		(filename[strlen(filename)-1] == 'o')) {
		return 1;
	}
	return 0;
}

/* returns a non-zero value if the given string
   contains the given character */
int contains_char(char * str, char search)
{
	int i;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == search) return 1;
	}
	return 0;
}

/* returns a non-zero value if the given file is a script */
int is_script(char * filename)
{
	FILE * fp;
	int found = 0;
	char linestr[BLOCK_SIZE];

	fp = fopen(filename,"r");
	if (!fp) return 0;

	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) < 2) continue;
			if ((linestr[0] == '#') &&
				(linestr[1] == '!')) {
				found = 1; 
			}
			break;
		}		
	}

	fclose(fp);

	if (found == 0) return 0;

	return 1;
}

/* returns a non-zero value of files matching the given
   pattern exist in the given directory */
int files_exist(char * directory, char * file_extension)
{
	char commandstr[BLOCK_SIZE];
	char temp_filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE];
	FILE * fp;
	int exists=0,valid_data=0;

	sprintf(temp_filename,"%s%cpm_temp_cmd",
			TEMP_DIRECTORY, DIRECTORY_SEPARATOR);

	sprintf(commandstr,"cd %s; ls %s%c*.%s > %s 2>%%1",
			TEMP_DIRECTORY, directory,
			DIRECTORY_SEPARATOR, file_extension,
			temp_filename);
	exists = system(commandstr);

	fp = fopen(temp_filename,"r");
	if (!fp) return exists;
	exists = -1;
	valid_data = 0;
	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			if (contains_char(linestr,':') != 0) {
				exists = 0;
				break;
			}
			else {
				valid_data = 1;
			}
		}
	}	
	fclose(fp);

	if ((exists == -1) && (valid_data==1)) {
		exists = 1;
	}
	else {
		exists = 0;
	}

	sprintf(commandstr,"%s %s",COMMAND_DELETE,temp_filename);
	if (system(commandstr)) return exists;
	return exists;
}

/* prints a script to the console so that it can be coppied */
void print_script(char * source_script, char * name)
{
	FILE * fp;
	int i, ctr;
	char linestr[BLOCK_SIZE],str[BLOCK_SIZE];

	fp = fopen(source_script, "r");
	if (!fp) return;

	printf("void save_%s(char * filename)\n{\n",name);
	printf("    FILE * fp;\n\n");
	printf("    fp = fopen(filename,\"w\");\n");
	printf("    if (!fp) return;\n");
	
	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			ctr = 0;
			for (i = 0; i < strlen(linestr); i++) {
				if ((linestr[i] != '\t') &&
					(linestr[i] != 10) &&
					(linestr[i] != 13)) {
					if (linestr[i] == '"') {
						str[ctr++] = '\\';
					}
					str[ctr++] = linestr[i];
				}
			}
			printf("    fprintf(fp,\"%%s\",\"%s\");\n", str);
		}
	}
	printf("    fclose(fp);\n}\n");

	fclose(fp);
}

/* returns the size of a directory */
void directory_size(char * directory,
					char * dir_size)
{
	char commandstr[BLOCK_SIZE];
	char temp_filename[BLOCK_SIZE];
	char linestr[BLOCK_SIZE];
	FILE * fp;
	int i;

	sprintf(temp_filename,"%s%cpm_dir_size",
			TEMP_DIRECTORY, DIRECTORY_SEPARATOR);

	sprintf(commandstr,"cd %s; du -sh > %s",
			directory,temp_filename); 

	i = system(commandstr);

	sprintf(dir_size,"%s","0K");
	if (file_exists(temp_filename)==0) return;

	fp = fopen(temp_filename,"r");
	if (!fp) return;
	
	while (!feof(fp)) {
		if (fgets(linestr, BLOCK_SIZE-1, fp) != NULL) {
			if (strlen(linestr) == 0) continue;
			for (i = 0; i < strlen(linestr); i++) {
				if ((linestr[i]=='\t') ||
					(linestr[i]==' ') ||
					(i == strlen(linestr)-1)) {
					if (i == strlen(linestr)-1) {
						dir_size[i] = linestr[i];
						i++;
					}
					dir_size[i]=0;
					break;
				}
				dir_size[i] = linestr[i];
			}
			break;
		}
	}
	fclose(fp);
}

/* Generates a chunk of bash script which alters
   the version number within other files.
   This saves having to change version numbers
   within multiple files. */
void script_version_numbers(FILE * fp,
							char * script_name)
{
	int i, first = 1;
	const int no_of_scripts = 5;
	char * scripts[] = {
		"debian","rpm","arch","puppy", "ebuild"
	};

	fprintf(fp, "%s", "\n# Update version numbers " \
			"automatically - so you don't have to\n");

	/* alter the version number from the previous one to
	   the current one */
	fprintf(fp, "%s", "sed -i 's/VERSION='" \
			"${PREV_VERSION}'/VERSION='${VERSION}'/g'" \
			" Makefile ");

	/* for each script except for the current one */
	for (i = 0; i < no_of_scripts; i++) {
		if (strcmp(scripts[i],script_name)==0) continue;
		if (first == 0) fprintf(fp,"%s"," ");
		fprintf(fp,"%s.sh",scripts[i]);
		first = 0;
	}
	fprintf(fp,"%s","\n");

	/* alter the version within the RPM spec file */
	fprintf(fp, "sed -i 's/Version: '${PREV_VERSION}'" \
			"/Version: '${VERSION}'/g' %s/${APP}.spec\n",
			RPM_SUBDIR);
	fprintf(fp, "sed -i 's/Release: '${RELEASE}" \
			"'/Release: '${RELEASE}'/g' %s/${APP}.spec\n",
			RPM_SUBDIR);

	/* alter the version within the Arch PKGBUILD file */
	fprintf(fp, "sed -i 's/pkgrel='${RELEASE}'/" \
			"pkgrel='${RELEASE}'/g' %s/PKGBUILD\n",
			ARCH_SUBDIR);
	fprintf(fp, "sed -i 's/pkgver='${PREV_VERSION}'/"	\
			"pkgver='${VERSION}'/g' %s/PKGBUILD\n",
			ARCH_SUBDIR);

	/* alter the version within the puppy pet.specs file */
	fprintf(fp, "sed -i \"s/-${PREV_VERSION}-" \
			"/-${VERSION}-/g\"" \
			" %s%cpet.specs\n",
			PUPPY_SUBDIR,
			DIRECTORY_SEPARATOR);
	fprintf(fp, "sed -i \"s/|${PREV_VERSION}|" \
			"/|${VERSION}|/g\"" \
			" %s%cpet.specs\n",
			PUPPY_SUBDIR,
			DIRECTORY_SEPARATOR);

	fprintf(fp,"%s","\n");
}
