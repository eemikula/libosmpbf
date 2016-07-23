#include <iostream>

#include "libosmpbf.h"

int main(int argc, char *argv[]){

	if (argc != 2){
		std::cout << "Usage: " << argv[0] << " [FILE]\n";
		return 0;
	}

	libosmpbf::PbfStream pbf(argv[1]);
	if (!pbf){
		std::cout << "Not Ok\n";
		return 1;
	}

	libosmpbf::PbfBlock block;
	while (pbf >> block){

		for (libosmpbf::PbfBlock::NodeIterator i = block.nodesBegin(); i != block.nodesEnd(); i.next()){

			const libosmpbf::BlockNode &node = *i;
			const std::string *name = 0;
			bool restaurant = false;

			for (int key = 0; key < node.tags(); key++){
				libosmpbf::BlockTag pair = node.tags(key);

				if (pair.first == "amenity" && pair.second == "restaurant")
					restaurant = true;
				else if (pair.first == "name")
					name = &pair.second;
			}

			if (restaurant && name)
				std::cout << "Restaurant: " << *name << "\n";

		}

		for (libosmpbf::PbfBlock::WayIterator i = block.waysBegin(); i != block.waysEnd(); i.next()){

			const libosmpbf::BlockWay &way = *i;
			const std::string *name = 0;
			bool restaurant = false;

			for (int key = 0; key < way.tags(); key++){
				libosmpbf::BlockTag pair = way.tags(key);

				if (pair.first == "amenity" && pair.second == "restaurant")
					restaurant = true;
				else if (pair.first == "name")
					name = &pair.second;
			}

			if (restaurant && name)
				std::cout << "Restaurant: " << *name << "\n";

		}

	}

	return 0;
}
