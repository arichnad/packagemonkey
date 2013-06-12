APP=packagemonkey
VERSION=0.10
ARCH_TYPE=`uname -m`

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o $(APP) src/*.c -Isrc
 
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o $(APP) src/*.c -Isrc

source:
	tar -cvzf ../$(APP)_$(VERSION).orig.tar.gz ../$(APP)-$(VERSION) --exclude-vcs

install:
	cp $(APP) $(DESTDIR)/usr/bin
	cp man/$(APP).1.gz $(DESTDIR)/usr/share/man/man1
	chmod 755 $(DESTDIR)/usr/bin/$(APP)
	chmod 644 $(DESTDIR)/usr/share/man/man1/$(APP).1.gz

clean:
	rm -f $(APP) \#* \.#* gnuplot* *.png debian/*.substvars debian/*.log
	rm -rf deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)
	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc
	rm -f rpmpackage/*.src.rpm
