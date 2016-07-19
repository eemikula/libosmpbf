TARGETS=osm.pb.o osm.pb.cc osm.pb.h
CFLAGS=

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

osm.pb.cc: osm.proto
	protoc --cpp_out=. osm.proto

osm.pb.h: osm.proto
	protoc --cpp_out=. osm.proto

osm.pb.o: osm.pb.cc osm.pb.h
	g++ -fPIC -c osm.pb.cc `pkg-config --cflags protobuf` $(CFLAGS)
