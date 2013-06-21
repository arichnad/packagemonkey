APP=packagemonkey
VERSION=0.10
RELEASE=1
ARCH_TYPE=`uname -m`

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o $(APP) src/*.c -Isrc -lz
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o $(APP) src/*.c -Isrc -lz
source:
	tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs
install:
	mkdir -m 755 -p $(DESTDIR)/usr/bin
	install -m 755 --strip $(APP) $(DESTDIR)/usr/bin
	mkdir -m 755 -p $(DESTDIR)/usr/share
	mkdir -m 755 -p $(DESTDIR)/usr/share/applications
	mkdir -m 755 -p $(DESTDIR)/usr/share/man
	mkdir -m 755 -p $(DESTDIR)/usr/share/man/man1
	install -m 644 man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1
	mkdir -m 755 -p $(DESTDIR)/usr/share/$(APP)
	mkdir -m 755 -p $(DESTDIR)/usr/share/pixmaps
	mkdir -m 755 -p $(DESTDIR)/usr/share/icons
	mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor
	mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/scalable
	mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps
	mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/24x24
	mkdir -m 755 -p $(DESTDIR)/usr/share/icons/hicolor/24x24/apps
	install -m 644 desktop/$(APP).desktop $(DESTDIR)/usr/share/applications/$(APP).desktop
	install -m 644 desktop/icon24.png $(DESTDIR)/usr/share/icons/hicolor/24x24/apps/$(APP).png
clean:
	rm -f $(APP) \#* \.#* gnuplot* *.png debian/*.substvars debian/*.log
	rm -rf deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)
	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc
	rm -f rpmpackage/*.src.rpm
