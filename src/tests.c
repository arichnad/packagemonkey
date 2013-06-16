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

#include "tests.h"

static int test_detect_project()
{
	char commandstr[BLOCK_SIZE];
	char directory[BLOCK_SIZE];
	char project_type[BLOCK_SIZE];
	int retval, i;

	const char * proj_type[] = {
		"c","cpp","py","vala"
	};

	printf("test_detect_project...");

	sprintf(directory,"%s%ctemp_5728",
			TEMP_DIRECTORY, DIRECTORY_SEPARATOR);
	if (directory_exists(directory) == 0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR, directory);
		retval = system(commandstr);
	}

	for (i = 0; i < 4; i++) {
		sprintf(commandstr,"%s %s%c*",
				COMMAND_DELETE, directory, DIRECTORY_SEPARATOR);
		retval = system(commandstr);

		sprintf(commandstr,"touch %s%ctest.%s",
				directory,
				DIRECTORY_SEPARATOR, proj_type[i]);
		retval = system(commandstr);

		project_type[0]=0;
		detect_project_type(directory, project_type);
		assert(strlen(project_type) > 0);
		assert(strcmp(project_type, proj_type[i]) == 0);
	}

	printf("Ok\n");

	return retval;
}

static void test_makefile()
{
	FILE * fp;
	int index,row;
	char filename[BLOCK_SIZE];

	printf("test_makefile...");

	/* create a test makefile */
	sprintf(filename,"%s%cpm_test_makefile",TEMP_DIRECTORY,DIRECTORY_SEPARATOR);

	fp = fopen(filename,"w");
	assert(fp);
	fprintf(fp,"all:\n");
	fprintf(fp,"\tgcc, and that\n");
	fprintf(fp,"debug:\n");
	fprintf(fp,"\tgcc in debug mode\n");
	fprintf(fp,"install:\n");
	fprintf(fp,"\tcd to some directory\n");
	fprintf(fp,"\tdo some other stuff\n");
	fprintf(fp,"\tmkdir crazydudes\n");
	fprintf(fp,"\tinstall -m 755 somefile someotherfile\n");
	fclose(fp);

	index = get_makefile_entry_from_file(filename,"debug", "gcc in debug mode");
	assert(index == 3);
	index = get_makefile_entry_from_file(filename,"debug", "1234");
	assert(index == -1);
	index = get_makefile_entry_from_file(filename,"install", "mkdir crazydudes");
	assert(index == 7);

	row = add_makefile_entry_to_file(filename, "debug", "other debug");
	index = get_makefile_entry_from_file(filename,"debug", "other debug");
	assert(index == 4);
	assert(index == row);
	row = add_makefile_entry_to_file(filename, "debug", "other debug");
	assert(row == -1);
	printf("Ok\n");
}

static void test_categories()
{
	char categories[BLOCK_SIZE];
	char main_category[BLOCK_SIZE];
	char additional_category[BLOCK_SIZE];

	printf("test_categories...");

	sprintf(categories,"%s","test1;test2");
	parse_desktop_category(categories, main_category, additional_category);
	assert(strcmp(main_category,"test1")==0);
	assert(strcmp(additional_category,"test2")==0);

	sprintf(categories,"%s","test1");
	parse_desktop_category(categories, main_category, additional_category);
	assert(strcmp(main_category,"test1")==0);
	assert(strlen(additional_category)==0);

	assert(valid_main_category("test123")==-1);
	assert(valid_main_category("Audio")!=-1);
	assert(valid_main_category("Utility")!=-1);

	assert(valid_additional_category("Profiling")!=-1);
	assert(valid_additional_category("Database")!=-1);
	assert(valid_additional_category("test123")==-1);
	assert(valid_additional_category("ConsoleOnly")!=-1);
	assert(valid_additional_category("foo")==-1);

	printf("Ok\n");
}

static void test_email()
{
	char email_address[BLOCK_SIZE];

	printf("test_email...");

	sprintf(email_address,"%s","Bob <user@domainname.com>");
	assert(valid_email(email_address) != 0);

	sprintf(email_address,"%s","Bob<user@domainname.com>");
	assert(valid_email(email_address) == 0);

	sprintf(email_address,"%s","Bob <user@domainname>");
	assert(valid_email(email_address) == 0);

	sprintf(email_address,"%s","user@domainname.com");
	assert(valid_email(email_address) == 0);

	sprintf(email_address,"%s","user");
	assert(valid_email(email_address) == 0);

	printf("Ok\n");
}

static void test_subdirectory()
{
	char directory[BLOCK_SIZE];
	char subdirectory[BLOCK_SIZE];
	char reference[BLOCK_SIZE];

	printf("test_subdirectory...");

	subdirectory[0]=0;
	sprintf(directory,"%s","/home/username/develop/myproject");
	get_subdirectory(directory,subdirectory);
	sprintf(reference,"myproject");
	assert(strcmp(subdirectory,reference)==0);

	subdirectory[0]=0;
	sprintf(directory,"%s","/home/username/develop/myproject/");
	get_subdirectory(directory,subdirectory);
	sprintf(reference,"myproject");
	assert(strcmp(subdirectory,reference)==0);

	subdirectory[0]=0;
	sprintf(directory,"%s","myproject");
	get_subdirectory(directory,subdirectory);
	sprintf(reference,"myproject");
	assert(strcmp(subdirectory,reference)==0);

	printf("Ok\n");
}

static void test_settings()
{
	char value[BLOCK_SIZE];
	char test1[BLOCK_SIZE];
	char test2[BLOCK_SIZE];

	sprintf(test1,"%.3f",1.234);
	sprintf(test2,"%.3f",2.567);

	value[0]=0;

	printf("test_settings...");

	add_setting("test1",test1);
	add_setting("test2",test2);

	get_setting("test2",value);
	assert(strcmp(value,test2)==0);

	get_setting("test1",value);
	assert(strcmp(value,test1)==0);

	printf("Ok\n");
}


void run_tests()
{
	test_settings();
	test_subdirectory();
	test_makefile();
	test_email();
	test_categories();
	test_makefile();
	test_detect_project();
}
