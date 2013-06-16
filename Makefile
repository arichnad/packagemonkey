APP=packagemonkey
VERSION=0.10
ARCH_TYPE=`uname -m`

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o $(APP) src/*.c -Isrc -lz
 
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o $(APP) src/*.c -Isrc -lz

source:
	tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs

install:
	install -m 755 --strip $(APP) $(DESTDIR)/usr/bin
	install -m 644 man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1
	mkdir -m 755 -p /usr/share/applications
	mkdir -m 755 -p /usr/share/applications/$(APP)
	mkdir -m 755 -p /usr/share/pixmaps
	mkdir -m 755 -p /usr/share/icons
	mkdir -m 755 -p /usr/share/icons/hicolor
	mkdir -m 755 -p /usr/share/icons/hicolor/scalable
	mkdir -m 755 -p /usr/share/icons/hicolor/scalable/apps
	mkdir -m 755 -p /usr/share/icons/hicolor/24x24
	mkdir -m 755 -p /usr/share/icons/hicolor/24x24/apps
	install -m 644 desktop/$(APP).desktop /usr/share/applications/$(APP)/$(APP).desktop
	install -m 644 desktop/icon24.png /usr/share/icons/hicolor/24x24/apps/$(APP).png
	install -m 644 desktop/icon.svg /usr/share/icons/hicolor/scalable/apps/$(APP).svg
	install -m 644 desktop/icon.svg /usr/share/pixmaps/$(APP).svg

clean:
	rm -f $(APP) \#* \.#* gnuplot* *.png debian/*.substvars debian/*.log
	rm -rf deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)
	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc
	rm -f rpmpackage/*.src.rpm
