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

clean:
	rm -f $(APP) \#* \.#* gnuplot* *.png debian/*.substvars debian/*.log
	rm -rf deb.* debian/$(APP) rpmpackage/$(ARCH_TYPE)
	rm -f ../$(APP)*.deb ../$(APP)*.changes ../$(APP)*.asc ../$(APP)*.dsc
	rm -f rpmpackage/*.src.rpm
