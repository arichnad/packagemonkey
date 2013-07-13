packagemonkey 
=============

<b>A tool to make packaging easier</b>

<img src="https://github.com/fuzzgun/packagemonkey/blob/master/img/packagemonkey320.png?raw=true" width=200/>

Description
-----------

Creating installable packages for Linux distros has always been a bit of a black art.  There is a lot of documentation out there and it's often voluminous and not especially easy to interpret.  When doing packaging by hand there are also many ways to go wrong.

Packagemonkey is intended to make packaging simpler, and enable the creation of multiple different types of package including deb, RPM, ebuild, Arch, PET and TXZ.  With a single command line statement you can generate all the necessary files and scripts, obeying all the various packaging rules.  When combined with Open Build Service this makes it easy to distribute software in a user friendly manner on many different distros. 

Installation
------------

The easiest way to install is from a pre-compiled package, a number of which are available at:

    https://build.opensuse.org/project/show?project=home%3Amotters%3Apackagemonkey

But if you prefer not to get involved with binaries then this program is pretty easy to compile and install as follows.

Check out the latest source code:

    git clone https://github.com/fuzzgun/packagemonkey.git

Then to build and install:

    cd packagemonkey
    make
    sudo make install

If you wish to generate packages for the packagemonkey application itself then run the generate.sh script.  This will generate a number of scripts:

    debian.sh
    rpm.sh
    arch.sh
    puppy.sh
    ebuild.sh
    slack.sh

Those scripts may then be run in order to generate packages.  The debian.sh script will generate a package within the directory one level above and the others create their own subdirectories within which the built packages can be found.


Example: Packaging some pre-existing binaries
---------------------------------------------

As the most likely kind of scenario it is assumed that you have somehow created some binaries or some scripts which you wish to turn into an installable package.

First create a directory for your package and give it the name of the project.  The project name should be all lower case, and not include the version number.

Create a subdirectory called "install", then insert your files within that directory using the same directory tree as you wish to be reproduced on the installed system.  For example if your project was called "foobar" the directory tree might look like the following:

    foobar
    foobar/install
    foobar/install/usr
    foobar/install/usr/bin
    foobar/install/usr/bin/foobar (the binary)

To create packaging files and scripts first make sure that you are in the foobar directory, then you could then run something like:

    packagemonkey --name "Foo Bar" --cmd --dir "." -l "gpl3" \
                  -e "Name <name@mydomainname.com>" \
                  --brief "Tool to make packaging easier" \
                  --desc "Making packing easier than it previously was." \
                  --homepage "https://github.com/fuzzgun/packagemonkey.git" \
                  --section "utils" --version "0.10" \
                  --categories "Utility/ConsoleOnly" --compile "-lz" \
                  --dependsdeb "build-essential, lintian"

For convenience it may be a good idea to put that into a script, so that it can easily be called again later without needing to be re-typed.

If you list the contents of the current directory (ls *.sh) you should then notice that a number of scripts have been created.

    debian.sh - create a Debian package
    rpm.sh - create an RPM package
    arch.sh - create a package for Arch Linux
    puppy.sh - create a package for Puppy Linux
    ebuild.sh - create a package for Gentoo
    slack.sh - create a package for Slackware

You can run any or all of these scripts to generate the relevant packages.  An important point to stress here is that with the exception of puppy.sh and ebuild.sh you will need to be running on a suitable system in order to create the package successfully.  So for example to create an RPM you will need to be running an RPM based distro (possibly as a virtual machine).  A handy alternative is to build packages using Open Build Service, which avoids needing to use a lot of virtual machines.

Packaging Scripts
-----------------

If you are packaging scripts written for an interpreted language, such as Python or Perl, then you don't need to use the install directory as in the previous example.  Put your script files into a subdirectory called src then run something like the following, substituting your own project details.

    packagemonkey -n "My Python Project" --cmd --dir "." -l "bsd" \
                  -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" \
                  --brief "Example Python Project Packaging" \
                  --desc "Example of how to use packagemonkey with a " \
				  "Python project" --homepage "https://myprojecturl" \
                  --repository "https://myprojectrepository.git" \
                  --section "utils" --version "0.1.2" \
                  --categories "Office/Email" \
                  --dependsdeb "python (>= 2.7.0)" --mainscript "main.py"

An important option is mainscript which determines which script should be called first.  So in the above example there is expected to exist a script called src/main.py

Packaging Libraries
-------------------

To package a C, C++ or Vala library put your code into a subdirectory named "src" and ensure that the name of the project and project directory name begins with "lib".  An example is as follows:

    packagemonkey -n "libgpr" --version "1.03" --cmd --dir "." -l "bsd" \
                  -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" \
                  --brief "Library for genetic programming" \
                  --desc "Making the inclusion of Genetic Programming " \
				  "easy within any C/C++ application. Genetic programming " \
				  "(GP) is a powerful technique, inspired by the process " \
				  "of natural selection, which can be utilized to " \
				  "automatically discover programs which produce a desired " \
				  "input to output transformation. Both classical " \
				  "tree-based and Cartesian forms of Genetic Programming " \
				  "are supported, including self-modifying variants." \
                  --homepage "https://github.com/fuzzgun/libgpr" \
				  --repository "https://github.com/fuzzgun/libgpr.git" \
				  --section "libs" --categories "Development/ArtificialIntelligence" \
                  --cstandard "c99" --compile "-lm -lz -fopenmp" \
                  --dependsdeb "gnuplot, libz-dev" --dependsarch "gnuplot, libzip"

Packaging GUI Applications
--------------------------

The above examples are all for command line applications. If you're packaging a project which has a GUI then simply omit the --cmd option. You'll need to figure out where to put your application within the various free desktop categories. To see what categories are available type:

   packagemonkey --list-desktop

You should pick a main and additional category, then include those within the --categories option when calling packagemonkey. For example:

   --categories "Utility/Email"

After generating packaging files you will notice that a blank PNG image appears within the desktop subdirectory. This is the desktop icon for your application, so you should edit it accordingly. For maximum compatibility with desktop environments such as Unity or Gnome Shell you should also add an SVG version of the icon within the desktop subdirectory. If you don't already have an SVG then one can be easily created by loading a PNG into Inkscape and then exporting it in SVG format.

If you intend to distribute your application on Puppy Linux then you will also need a mini-icon. This should be a 14x14 pixel icon saved in XPM format within the desktop subdirectory.

Once you have added an SVG or mini-icon to the desktop directory then you should regenerate the packaging again so that the Makefile is updated. 

Changelog
---------

Once packaging files have been generated then the master changelog is the one within the debian subdirectory.  Changes made to debian/changelog will update the RPM spec file automatically if you subsequently run packagemonkey again to regenerate the files.

Using Packagemonkey with Open Build Service
-------------------------------------------

Open Build Service (OBS) allows you to easily create packages for multiple distros.  The only down side is that it doesn't do the hard part of creating all the necessary packaging files.  However, when you combine OBS with packagemonkey this results in more of an end-to-end packaging solution.

If you're creating Debian packages then upload:

    ../foobar_version.orig.tar.gz
    ../foobar_version.orig.tar.gz.asc
    ../foobar_version-1.debian.tar.gz
    ../foobar_version-1.dsc
    
If you are creating RPM packages then upload:

    rpmpackage/foobar.spec
    ../foobar_version.orig.tar.gz

If you are creating Arch packages then upload:

    archpackage/PKGBUILD
    archpackage/foobar-version.tar.gz

Why don't we upload the source file created by running the Debian script? OBS seems to expect Arch source packages to have a specific filename format, without the "orig" and with a dash instead of an underscore.  Building Arch packages also typically only seems to work if you select the "Arch Extra" rather than the "Arch Core" repository.

Options
-------

<b>-n, --name [project name]</b><br>
The full name or title of the project.  This may contain upper case characters, and may be different from the name of the project directory.

<b>-d, --dir [directory]</b><br>
Set the directory of the project to be packaged.  If this is set to "." then the current directory will be used.

<b>-c, --cmd</b><br>
Used to indicate this is a command line project.  If this option is not used then an icon and desktop file suitable for a GUI application will be generated.

<b>-b, --binaries [files]</b><br>
Directory containing binaries or scripts to be packaged.  By default this is a subdirectory called "install".

<b>--compile [args]</b><br>
Additional arguments to add to the compilation.

<b>-l, --license [type]</b><br>
The license type (gpl2|gpl3|lgpl2|lgpl3|agpl|agpl3|mit|bsd|apache|mozilla)

<b>-e, --email [address]</b><br>
Set full name and email address.

<b>--brief [description]</b><br>
A brief one line description of the project.

<b>--desc [description]</b><br>
A full description of the project.

<b>--homepage [url]</b><br>
URL of the project home page.

<b>--repository [url]</b><br>
URL of the version control repository.

<b>--srcdir [directory]</b><br>
Subdirectory within which source code will be located.  This is typically "src".  The directory should only be one level deep.

<b>--src [url]</b><br>
URL of the compressed source code.

<b>--section [sections]</b><br>
Debian sections.

<b>--categories [cat]</b><br>
desktop file categories.

<b>-m, --mime [types]</b><br>
Mime types which the project can handle.

<b>--dependsdeb [packages]</b><br>
Deb packages which the project depends upon.

<b>--builddeb [packages]</b><br>
Deb packages required to build the project.

<b>--dependsrpm [packages]</b><br>
RPM packages which the project depends upon.

<b>--dependsrpmdistro [packages]</b><br>
Distro-specific RPM packages which the project depends upon.  The first entry should be the name of the distro.

For example:

    --dependsrpm "sqlite"
    --dependsrpmdistro "mandriva, sqlite3"

This will use the package "sqlite" by default, but if the operating system is Mandriva then it will use the "sqlite3" package instead.

<b>--dependsarch [packages]</b><br>
Arch packages which the project depends upon.

<b>--dependspuppy [packages]</b><br>
Puppy packages which the project depends upon.

<b>--dependsebuild [packages]</b><br>
Ebuild packages which the project depends upon.

<b>--buildrpm [packages>]</b><br>
RPM packages required to build the project.

<b>--buildrpmdistro [packages>]</b><br>
Distro-specific RPM packages which the project depends upon.  The first entry should be the name of the distro.

For example:

    --buildrpm "sqlite-devel"
    --buildrpmdistro "mandriva, libsqlite3-devel"

This will use the package "sqlite-devel" by default, but if the operating system is Mandriva then it will use the "libsqlite3-devel" package instead.

<b>--buildarch [packages>]</b><br>
Arch packages required to build the project.

<b>--list-sections</b><br>
List the possible Debian sections.

<b>--list-desktop</b><br>
List the possible desktop file categories.

<b>--debian [version]</b><br>
Set the Debian version.

<b>--mainscript [filename]</b><br>
If the project uses a scripted language, such as Python or Perl, then this specifies the initial script to be run.  This should be just the filename and not include the full path.

<b>--cstandard [standard]</b><br>
The standard to be used by the gcc or g++ compiler.  Examples are c99 and gnu99.

<b>--librarypath [paths]</b><br>
Specify LD_LIBRARY_PATH.

<b>--suggestsdeb</b><br>
Suggested Debian packages.

<b>--suggestsarch</b><br>
Suggested Arch packages.

<b>-t, --tests</b><br>
Run unit tests.

<b>-v, --version [number]</b><br>
Show or set the project version number.  If only a single option exists then this will return the version number of packagemonkey, otherwise in a multi-option situation it can be used to set the version number of the prokect to be packaged.

<b>-r, --release [number]</b><br>
Set the project release number.

<b>-h, --help</b><br>
Show help.


Bugs
----

Report bugs at https://github.com/fuzzgun/packagemonkey/issues
