TARGETS=../lib/libosmpbf.so ../lib/libosmpbf.a
CFLAGS=

all: $(TARGETS)
	@$(MAKE) -C protobuf

clean:
	rm -f $(TARGETS) libosmpbf.o
	@$(MAKE) clean -C protobuf

protobuf/osm.pb.o:
	@$(MAKE) -C protobuf

protobuf/osm.pb.h:
	@$(MAKE) -C protobuf

libosmpbf.o: libosmpbf.cpp ../include/libosmpbf.h protobuf/osm.pb.h
	g++ -fPIC -c libosmpbf.cpp `pkg-config --cflags protobuf zlib` $(CFLAGS) -I../include

../lib/libosmpbf.so: libosmpbf.o protobuf/osm.pb.o
	mkdir -p ../lib
	g++ -shared -Wl,-soname,libosmpbf.so -o ../lib/libosmpbf.so libosmpbf.o protobuf/osm.pb.o `pkg-config --libs protobuf zlib`

../lib/libosmpbf.a: libosmpbf.o protobuf/osm.pb.o
	mkdir -p ../lib
	ar rcs ../lib/libosmpbf.a libosmpbf.o protobuf/osm.pb.o
