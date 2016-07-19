/*
The MIT License (MIT)

Copyright (c) 2016 Eric Mikula

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LIBOSMPBF_H
#define LIBOSMPBF_H

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <stdint.h>

namespace OSMPBF {
	class Blob;
	class Node;
	class PrimitiveBlock;
	class Relation;
	class Way;
}

namespace libpbf {

class PbfStream;
class PbfBlock;
class BlockNode;
class BlockWay;
class BlockRelation;
class Node;
class Way;
class Relation;

class PbfStream : public std::fstream {
public:
	PbfStream(const char *file);
	~PbfStream();

	std::fstream &operator >> (PbfBlock &block);

private:

	std::fstream &readDataStr(std::fstream &in, std::string &str, size_t size);
	std::fstream &readBlob(std::fstream &in, OSMPBF::Blob &blob);
	bool inflate(std::string data, unsigned char *buf, size_t bufSz);

	template <typename T>
	bool getCompressedBlock(OSMPBF::Blob &blob, T &block);

};

// container for tag name and value pairs tied to PrimitiveBlock
typedef std::pair<const std::string &, const std::string &> BlockTag;

// container for tag name and value pairs not tied to PrimitveBlock
typedef std::pair<std::string, std::string> Tag;

// container for latitude and longitude pairs
struct Coords {
public:

	// constructs floating-point latitude and longitude using the integer
	// values stored in PBF and the relevant granularity
	Coords(int64_t lat, int64_t lon, int32_t granularity);
	Coords();
	double lat, lon;
};

// This structure holds a copy of data contained within a BlockNode that is
// not tied to the data structure on disk, and can persist after the PrimitiveBlock
// is no longer valid
struct Node {
	Node();
	Node(const BlockNode &n);
	uint64_t id;
	Coords coords;
	std::map<std::string, std::string> tags;
};

class BlockNode {
public:
	BlockNode(const OSMPBF::PrimitiveBlock &b, int group, bool dense, int i, uint64_t idBase, uint64_t node);
	BlockNode(const OSMPBF::PrimitiveBlock &b, int group, int i);

	uint64_t id() const;
	int tags() const;
	BlockTag tags(int x) const;
	Coords coords() const;

	Node clone() const;

private:
	const OSMPBF::PrimitiveBlock &block;
	int group, i, node;
	uint64_t idBase;
	bool dense;
};

struct Way {
	Way();
	Way(const BlockWay &w);
	uint64_t id;
	std::list<uint64_t> nodeIds;
	std::map<std::string, std::string> tags;
	std::list<Node> nodes;
};

class BlockWay {
public:
	BlockWay(const OSMPBF::Way &w, const OSMPBF::PrimitiveBlock &b);

	uint64_t id() const;
	int tags() const;
	BlockTag tags(int i) const;
	int nodes() const;
	uint64_t nodes(int i) const;

	Way clone() const;

private:
	const OSMPBF::Way &way;
	const OSMPBF::PrimitiveBlock &block;
};

enum MemberType {
	Member_Node = 0,
	Member_Way = 1,
	Member_Relation = 2
};

struct Relation {

	struct Member {
		Member(uint64_t id, MemberType type, const std::string &r);
		uint64_t id;
		MemberType type;
		std::string role;
	};

	Relation(const BlockRelation &r);
	uint64_t id;
	std::list<Member> members;
	std::map<std::string, std::string> tags;
};

class BlockRelation {
public:

	struct Member {
		Member(uint64_t id, MemberType type, const std::string &r);
		uint64_t id;
		MemberType type;
		const std::string &role;
	};

	BlockRelation(const OSMPBF::Relation &r, const OSMPBF::PrimitiveBlock &b);

	uint64_t id() const;
	int tags() const;
	BlockTag tags(int i) const;
	int members() const;
	Member members(int i) const;

	Relation clone() const;

private:
	const OSMPBF::Relation &relation;
	const OSMPBF::PrimitiveBlock &block;
};

class PbfBlock {
public:

	PbfBlock();
	~PbfBlock();

	class NodeIterator {
	public:
		NodeIterator(OSMPBF::PrimitiveBlock &b, bool end);

		bool hasData() const;

		bool operator == (const NodeIterator &i) const;
		bool operator != (const NodeIterator &i) const;

		NodeIterator &next();

		const BlockNode operator -> () const;
		const BlockNode operator * () const;

	private:
		OSMPBF::PrimitiveBlock &block;
		int group, i, node;
		uint64_t idBase;
		bool dense, end;
	};

	class WayIterator {
	public:
		WayIterator(OSMPBF::PrimitiveBlock &b, bool end);

		bool hasData() const;
		bool operator == (const WayIterator &i) const;
		bool operator != (const WayIterator &i) const;

		WayIterator &next();

		const BlockWay operator -> () const;
		const BlockWay operator * () const;

	private:
		OSMPBF::PrimitiveBlock &block;
		int group, i;
		bool end;
	};

	class RelationIterator {
	public:
		RelationIterator(OSMPBF::PrimitiveBlock &b, bool end);

		bool hasData() const;

		bool operator == (const RelationIterator &i) const;
		bool operator != (const RelationIterator &i) const;

		RelationIterator &next();

		const BlockRelation operator -> () const;
		const BlockRelation operator * () const;

	private:
		OSMPBF::PrimitiveBlock &block;
		int group, i;
		bool end;
	};

	int granularity() const;

	int Nodes() const;
	NodeIterator nodesBegin();
	NodeIterator nodesEnd();
	int Ways() const;
	WayIterator waysBegin();
	WayIterator waysEnd();
	int Relations() const;
	RelationIterator relationsBegin();
	RelationIterator relationsEnd();

private:

	friend std::fstream &PbfStream::operator >> (PbfBlock &block);
	OSMPBF::PrimitiveBlock *block;

};

} // end namespace

#endif
