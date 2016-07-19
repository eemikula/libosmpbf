all: example_static example_dynamic

clean:
	rm -f example_static example_dynamic

example_static: example.cpp ../lib/libosmpbf.a
	g++ -o example_static example.cpp ../lib/libosmpbf.a -I../include/ `pkg-config --libs protobuf zlib`

example_dynamic: example.cpp
	g++ -o example_dynamic example.cpp -losmpbf -I../include/ `pkg-config --libs protobuf zlib`

