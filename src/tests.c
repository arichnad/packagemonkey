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
}
