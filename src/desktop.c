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

#include "desktop.h"

const int no_of_main_categories = 13;
char * category_main[] = {
	"AudioVideo","Audio","Video","Development","Education",
	"Game","Graphics","Network","Office","Science",
	"Settings","System","Utility"
};

const int no_of_additional_categories = 156-30;
char * category_additional[] = {
	"Building",
	"Debugger",
	"IDE",
	"GUIDesigner",
	"Profiling",
	"RevisionControl",
	"Translation",
	"Calendar",
	"ContactManagement",
	"Database",
	"Dictionary",
	"Chart",
	"Email",
	"Finance",
	"FlowChart",
	"PDA",
	"ProjectManagement",
	"Presentation",
	"Spreadsheet",
	"WordProcessor",
	"2DGraphics",
	"VectorGraphics",
	"RasterGraphics",
	"3DGraphics",
	"Scanning",
	"OCR",
	"Photography",
	"Publishing",
	"Viewer",
	"TextTools",
	"DesktopSettings",
	"HardwareSettings",
	"Printing",
	"PackageManager",
	"Dialup",
	"InstantMessaging",
	"Chat",
	"IRCClient",
	"Feed",
	"FileTransfer",
	"HamRadio",
	"News",
	"P2P",
	"RemoteAccess",
	"Telephony",
	"TelephonyTools",
	"VideoConference",
	"WebBrowser",
	"WebDevelopment",
	"Midi",
	"Mixer",
	"Sequencer",
	"Tuner",
	"TV",
	"AudioVideoEditing",
	"Player",
	"Recorder",
	"DiscBurning",
	"ActionGame",
	"AdventureGame",
	"ArcadeGame",
	"BoardGame",
	"BlocksGame",
	"CardGame",
	"KidsGame",
	"LogicGame",
	"RolePlaying",
	"Shooter",
	"Simulation",
	"SportsGame",
	"StrategyGame",
	"Art",
	"Construction",
	"Music",
	"Languages",
	"ArtificialIntelligence",
	"Astronomy",
	"Biology",
	"Chemistry",
	"ComputerScience",
	"DataVisualization",
	"Economy",
	"Electricity",
	"Geography",
	"Geology",
	"Geoscience",
	"History",
	"Humanities",
	"ImageProcessing",
	"Literature",
	"Maps",
	"Math",
	"NumericalAnalysis",
	"MedicalSoftware",
	"Physics",
	"Robotics",
	"Spirituality",
	"Sports",
	"ParallelComputing",
	"Amusement",
	"Archiving",
	"Compression",
	"Electronics",
	"Emulator",
	"Engineering",
	"FileTools",
	"FileManager",
	"TerminalEmulator",
	"Filesystem",
	"Monitor",
	"Security",
	"Accessibility",
	"Calculator",
	"Clock",
	"TextEditor",
	"Documentation",
	"Adult",
	"Core",
	"KDE",
	"GNOME",
	"XFCE",
	"GTK",
	"Qt",
	"Motif",
	"Java",
	"ConsoleOnly"
};

char * puppy_hierarchy[] = {
	"Desktop",    "appearance",     "X-Desktop-appearance,DesktopSettings",
	"Desktop",    "settings",       "X-Desktop-settings,X-DesktopCountry,Accessibility",
	"Desktop",    "windows",        "X-Desktop-windows",
	"Desktop",    "sleep",          "X-Desktop-sleep,Screensaver",
	"Desktop",    "applet",         "X-Desktop-applet,X-DesktopApplets,Clock",

	"System",     "Sub",            "X-System,System,HardwareSettings,Core",
	"System",     "process",        "X-System-process,Monitor,X-SystemSchedule",
	"System",     "memory",         "X-System-memory,X-SystemMemory",
	"System",     "print",          "X-System-print",
	"System",     "storage",        "X-System-storage",
	"System",     "security",       "X-System-security,Security",

	"Setup",      "Sub",            "X-Setup",
	"Setup",      "puppy",          "X-Setup-puppy,X-SetupEntry,PackageManager",
	"Setup",      "wizard",         "X-Setup-wizard,X-SetupWizard",
	"Setup",      "installation",   "X-Setup-installation,X-SetupUtility",

	"Utility",    "Sub",            "X-Utility,Utility,Electronics,Emulator,Engineering",
	"Utility",    "development",    "X-Utility-development,Development,Building,Debugger,IDE,Profiling,ProjectManagement,RevisionControl,Translation,GUIDesigner",
	"Utility",    "package",        "X-Utility-package,Archiving,Compression",
	"Utility",    "shell",          "X-Utility-shell,TerminalEmulator,Shell",
	"Utility",    "help",           "X-Utility-help,Documentation",

	"Filesystem", "Sub",            "X-Filesystem,FileSystem,FileTools",
	"Filesystem", "filemanager",    "X-Filesystem-filemanager,FileManager",
	"Filesystem", "mount",          "X-Filesystem-mount,X-FilesystemMount", 
	"Filesystem", "find",           "X-Filesystem-find,X-FilesystemFind",
	"Filesystem", "storage",        "X-Filesystem-storage,X-FilesystemUtility",

	"Graphic",    "Sub",            "X-Graphic,2DGraphics,3DGraphics",
	"Graphic",    "paint",          "X-Graphic-paint,RasterGraphics2",
	"Graphic",    "draw",           "X-Graphic-draw,VectorGraphics",
	"Graphic",    "flow",           "X-Graphic-flow,Chart,FlowChart,Presentation",
	"Graphic",    "viewer",         "X-Graphic-viewer,Viewer,X-Graphic-viewer",
	"Graphic",    "camera",         "X-Graphic-camera,Photography",
	"Graphic",    "scanner",        "X-Graphic-scanner,Scanning,OCR",
	"Graphic",    "print",          "X-Graphic-print",
	"Graphic",    "utility",        "X-Graphic-utility,X-GraphicUtility",

	"Document",   "Sub",            "X-Document",
	"Document",   "layout",         "X-Document-layout,Publishing,WordProcessor,WebDevelopment",
	"Document",   "catalog",        "X-Document-catalog,Dictionary",
	"Document",   "edit",           "X-Document-edit,TextEditor",
	"Document",   "viewer",         "X-Document-viewer,Viewer",
	"Document",   "utility",        "X-Document-utility,X-DocumentUtility",

	"Business",   "Sub",            "X-Business,X-Calculate2",
	"Business",   "spreadsheet",    "X-Business-spreadsheet,Spreadsheet",
	"Business",   "planner",        "X-Business-planner,ProjectManagement",
	"Business",   "finance",        "X-Business-finance,Finance",
	"Business",   "calc",           "X-Business-calc,Calculator,X-CalculateUtility",

	"Personal",   "Sub",            "X-Personal",
	"Personal",   "database",       "X-Personal-database,ContactManagement",
	"Personal",   "date",           "X-Personal-date,Calendar",
	"Personal",   "organizer",      "X-Personal-organizer,PDA",
	"Personal",   "security",       "X-Personal-security",
	"Personal",   "timing",         "X-Personal-timing,X-PersonalUtility",
	"Personal",   "education",      "X-Personal-education,Languages,Science,ArtificialIntelligence,Astronomy,Biology,Chemistry,ComputerScience,DataVisualization,Economy,Electricity,Geography,Geology,Geoscience,History,ImageProcessing,Literature,Math,NumericalAnalysis,MedicalSoftware,Physics,Robotics,Sports,ParallelComputing",

	"Network",    "Sub",            "X-Network,Network",
	"Network",    "firewall",       "X-Network-firewall",
	"Network",    "phone",          "X-Network-phone,Dialup",
	"Network",    "connect",        "X-Network-connect,HamRadio,RemoteAccess",
	"Network2",   "transfer",       "X-Network-transfer",
	"Network",    "utility",        "X-Network-utility",

	"Internet",   "Sub",            "X-Internet",
	"Internet",   "browser",        "X-Internet-browser,WebBrowser",
	"Internet",   "chat",           "X-Internet-chat,Chat,InstantMessaging,VideoConference  #ayttm pidgin pidgin_perl psip wengophone xchat",
	"Internet",   "mailnews",       "X-Internet-mailnews,Email,News",
	"Internet",   "transfer",       "X-Internet-transfer,Telephony,IRCClient,FileTransfer,P2P #axel bareftp gcurl gftp lftp ncftp puppy-podcast-grabber pwget",
	"Internet",   "login",          "X-Internet-login",
	"Internet",   "block",          "X-Internet-block",
	"Internet",   "utility",        "X-Internet-utility",

	"Multimedia", "Sub",            "X-Multimedia,Recorder",
	"Multimedia", "mediaplayer",    "X-Multimedia-mediaplayer,Player",
	"Multimedia", "audiovideo",     "X-Multimedia-audiovideo,AudioVideo,AudioVideoEditing,Tuner",
	"Multimedia", "sound",          "X-Multimedia-sound,Music,Audio,Midi,Mixer,Sequencer #absvolume absvolume_puppy audacity aumix madplay mhwaveedit sgmixer",
	"Multimedia", "video",          "X-Multimedia-video,Video,TV",
	"Multimedia", "optical",        "X-Multimedia-optical,DiskBurning",
	"Multimedia", "utility",        "X-Multimedia-utility",

	"Fun",        "Sub",            "X-Fun,Game,Amusement,BlocksGame,KidsGame,Simulation,SportsGame",
	"Fun",        "adventure",      "X-Fun-adventure,ActionGame,AdventureGame,RolePlaying",
	"Fun",        "boardgame",      "X-Fun-boardgame,BoardGame",
	"Fun",        "cardgame",       "X-Fun-cardgame,CardGame",
	"Fun",        "puzzle",         "X-Fun-puzzle,StrategyGame,LogicGame",
	"Fun",        "shooter",        "X-Fun-shooter",
	"Fun",        "arcade",         "X-Fun-arcade,ArcadeGame",

	"End",        "",               ""
};

/* converts free desktop categories into puppy desktop categories */
void free_desktop_to_puppy_desktop(char * free_desktop_categories,
								   char * puppy_desktop_categories)
{
	char free_main_category[BLOCK_SIZE];
	char free_additional_category[BLOCK_SIZE];
	int i = 0;

	sprintf(puppy_desktop_categories,"%s","Desktop");

	parse_desktop_category(free_desktop_categories,
						   free_main_category,
						   free_additional_category);

	while (strcmp(puppy_hierarchy[i],"End") != 0) {
		if (strstr(puppy_hierarchy[i+2],
				   free_additional_category) != NULL) {
			sprintf(puppy_desktop_categories,
					"%s;%s;",
					puppy_hierarchy[i],
					puppy_hierarchy[i+1]);
			break;
		}
		i += 3;
	}
}

/* parses the given categories string into the main and additional parts */
void parse_desktop_category(char * str, char * main, char * additional)
{
	int i, ctr=0;
	char * array = main;

	main[0] = 0;
	additional[0] = 0;

	for (i = 0; i < strlen(str); i++) {
		if ((str[i] != ';') &&
			(str[i] != ',') &&
			(str[i] != '/') &&
			(i != strlen(str)-1)) {
			array[ctr++] = str[i];
		}
		else {
			if (i == strlen(str)-1) {
				if ((str[i] != ';') &&
					(str[i] != ',') &&
					(str[i] != '/')) {
					array[ctr++] = str[i];
				}
			}
			array[ctr] = 0;
			array = additional;
			ctr = 0;
		}
	}
}

/* returns the matching main category array index or -1 of not found */
int valid_main_category(char * category)
{
	int i;
	char str0[BLOCK_SIZE], str1[BLOCK_SIZE];

	string_to_lower(category,str1);
	for (i = 0; i < no_of_main_categories; i++) {
		string_to_lower(category_main[i], str0);
		if (strcmp(str0,str1) == 0) {
			return i;
		}
	}
	return -1;
}

/* returns the matching additional category array index or -1 of not found */
int valid_additional_category(char * category)
{
	int i;
	char str0[BLOCK_SIZE], str1[BLOCK_SIZE];

	string_to_lower(category,str1);

	for (i = 0; i < no_of_additional_categories; i++) {
		string_to_lower(category_additional[i], str0);
		if (strcmp(str0,str1) == 0) {
			return i;
		}
	}
	return -1;
}

/* Shows the possible categories */
void show_categories()
{
	int i,ctr=0;

	printf("Main Categories:\n");
	for (i = 0; i < no_of_main_categories; i++) {
		printf("  %s",category_main[i]);
		if (ctr > 3) {
		    printf("\n");
		    ctr = 0;
	    }
		ctr++;
	}
	ctr = 0;
	printf("\n\nAdditional Categories:\n");
	for (i = 0; i < no_of_additional_categories; i++) {
		printf("  %s",category_additional[i]);
		if (ctr > 3) {
		    printf("\n");
		    ctr = 0;
	    }
		ctr++;
	}
	printf("\n");
}

/* Shows the possible main categories */
void show_categories_main()
{
	int i,ctr=0;

	printf("Main Categories:\n");
	for (i = 0; i < no_of_main_categories; i++) {
		printf("  %s",category_main[i]);
		if (ctr > 3) {
		    printf("\n");
		    ctr = 0;
	    }
		ctr++;
	}
	printf("\n");
}

/* Shows the possible additional categories */
void show_categories_additional()
{
	int i,ctr=0;

	printf("\n\nAdditional Categories:\n");
	for (i = 0; i < no_of_additional_categories; i++) {
		printf("  %s",category_additional[i]);
		if (ctr > 3) {
		    printf("\n");
		    ctr = 0;
	    }
		ctr++;
	}
	printf("\n");
}

static int write_png_file(char* filename, int width, int height, unsigned char *buffer)
{
	png_t png;
	FILE * fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		return 1;
	}
	fclose(fp);  

	png_init(0,0);
	png_open_file_write(&png, filename);
	png_set_data(&png, width, height, 8, PNG_TRUECOLOR, buffer);
	png_close_file(&png);

	return 0;
}

static void save_icon(char * filename, int resolution)
{
	unsigned char * buffer;

	if (file_exists(filename) != 0) return;

	buffer = (unsigned char *)malloc(resolution*resolution*3);
	memset((void*)buffer, '\255', resolution*resolution*3);
	write_png_file(filename, resolution, resolution, buffer);
	free(buffer);
}

int save_desktop()
{
	FILE * fp;
	char directory[BLOCK_SIZE];
	char commandstr[BLOCK_SIZE];
	char desktopdir[BLOCK_SIZE];
	char icon_filename[BLOCK_SIZE];
	char desktop_filename[BLOCK_SIZE];
	char project_name[BLOCK_SIZE];
	char project_full_name[BLOCK_SIZE];
	char description_brief[BLOCK_SIZE];
	char desktop_categories[BLOCK_SIZE];
	char category_main[BLOCK_SIZE];
	char category_additional[BLOCK_SIZE];
	char commandline[BLOCK_SIZE];
	int retval=0;

	/* if this is a command line project then we don't
	   need desktop files */
	get_setting("commandline",commandline);
	if (strlen(commandline) > 0) return retval;

	get_setting("directory",directory);
	get_setting("project name",project_name);
	get_setting("project full name",project_full_name);
	get_setting("description brief",description_brief);
	get_setting("categories",desktop_categories);

	/* create a desktop directory */
	sprintf(desktopdir,"%s%cdesktop",
			directory, DIRECTORY_SEPARATOR);
	if (directory_exists(desktopdir)==0) {
		sprintf(commandstr,"%s %s",COMMAND_MKDIR,desktopdir);
		retval = system(commandstr);
	}

	/* create a 24x24 icon */
	sprintf(icon_filename,"%s%cicon24.png",
			desktopdir, DIRECTORY_SEPARATOR);
	save_icon(icon_filename, 24);

	/* create a desktop file */
	sprintf(desktop_filename,"%s%c%s.desktop",
			desktopdir, DIRECTORY_SEPARATOR, project_name);

	fp = fopen(desktop_filename,"w");
	if (fp) {
		fprintf(fp,"[Desktop Entry]\n");
		fprintf(fp,"Type=Application\n");
		fprintf(fp,"Name=%s\n",project_full_name);
		fprintf(fp,"GenericName=%s\n",project_full_name);
		fprintf(fp,"Comment=%s\n",description_brief);
		fprintf(fp,"Exec=%s %%U\n",project_name);
		fprintf(fp,"Icon=%s\n",project_name);
		fprintf(fp,"Terminal=false\n");
		if (strlen(desktop_categories) == 0) {
			fprintf(fp,"Categories=Utility;\n");
		}
		else {
			parse_desktop_category(desktop_categories,
								   category_main,
								   category_additional);

			fprintf(fp,"Categories=%s;%s;\n",
					category_main, category_additional);
		}
		fclose(fp);
	}
	return retval;
}

