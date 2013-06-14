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

/* returns a non-zero value if the given license is valid */
int valid_license(char * license)
{
	int i;

	/* convert to lower case */
	for (i = 0; i < strlen(license); i++) {
		license[i] = tolower(license[i]);
	}

	/* check against a list of known licenses */
	if ((strcmp(license,"gpl2")==0) ||
		(strcmp(license,"gpl3")==0) ||
		(strcmp(license,"mit")==0) ||
		(strcmp(license,"bsd")==0) ||
		(strcmp(license,"apache")==0)) {
		return 1;
	}
	return 0;
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
